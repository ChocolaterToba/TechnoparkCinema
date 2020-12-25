#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>

#include "ports.hpp"

#include "TaskFuncs.hpp"
#include "HTTPClient.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpRequestCreator.hpp"
#include "TemplateManager.hpp"


MainFuncType PreProcess(map<string, string>& headers, vector<char>& body, HTTPClient& input) {
    input.recvHeader();

    int bodySize = 0;
    headers.clear();
    if (input.getPort() == FROM_DB_PORT) {  // port FROM_DB_PORT is reserved for db's responses
        HttpRequest request(input.getHeader()); // TODO: replace with HttpResponseReader.
        headers = request.GetAllHeaders();
        headers["url"] = request.GetURL();
        headers["method"] = request.GetRequestMethodString();
        headers["http_version"] = request.GetHTTPVersion();
        bodySize = request.GetContentLength();
    } else {
        HttpRequest request(input.getHeader());
        headers = request.GetAllHeaders();
        headers["url"] = request.GetURL();
        headers["method"] = request.GetRequestMethodString();
        headers["http_version"] = request.GetHTTPVersion();
        bodySize = request.GetContentLength();
    }

    body.clear();
    if (bodySize > 0) {
        input.recvBody(bodySize);
        body = input.getBody();
    }

    if (input.getPort() == FROM_DB_PORT)
        return MainProcessDBReceived;
    else if (input.getPort() == TO_DB_PORT)
        return MainProcessDBServer;

    return MainProcessBasic;
}

void MainProcessBasic(map<string, string>& headers, vector<char>& body,
                      map<int, HTTPClient>& pendingDBResponse,
                      std::shared_ptr<std::mutex> pendingDBResponseMutex,
                      HTTPClient& input, HTTPClient& output) {
    ContentType type = HttpResponse::GetContentType(headers["url"]);
    if (type == TXT_HTML) {
        TemplateManager templateManager(headers["url"]);
        std::set<string> params = templateManager.GetParameterNames();
        if (params.empty()) {
            body = templateManager.GetHtmlFinal(map<string, string>());

            output = std::move(input);
        } else {
            pendingDBResponseMutex->lock();
            pendingDBResponse.insert(std::pair<int, HTTPClient&>(input.getSd(), input));
            pendingDBResponseMutex->unlock();

            headers["Connection"] = "close";  // maybe make headers from scratch???

            body.clear();
            string sdString = std::to_string(input.getSd());
            body.insert(body.end(), sdString.begin(), sdString.end());
            body.push_back('|');
            vector<char> paramsPart = HTTPClient::mergeSetToVector(params);
            body.insert(body.end(), paramsPart.begin(), paramsPart.end());

            output = HTTPClient("localhost", TO_DB_PORT);
            headers["proxy"] = "true";
        }
    } else {
        std::ifstream source("../static" + headers["url"], std::ios::binary);
        char buffer[BUF_SIZE] = {0};
        while (source.read(buffer, BUF_SIZE))
            body.insert(body.end(), buffer, buffer + BUF_SIZE);

        body.insert(body.end(), buffer, buffer + source.gcount());

        output = std::move(input);
    }
}
static map<string, string> ProcessTemplatesInDB(const set<string> &params, size_t ID);
void MainProcessDBServer(map<string, string>& headers, vector<char>& body,
                         map<int, HTTPClient>& pendingDBResponse,
                         std::shared_ptr<std::mutex> pendingDBResponseMutex,
                         HTTPClient& input, HTTPClient& output) {
    auto firstSepPos = std::find(body.begin(), body.end(), '|');
    if (firstSepPos > body.end())
        firstSepPos = body.end();

    TemplateManager templateManager(headers["url"]);
    size_t id = 0;
    size_t find = headers["url"].find("watch?");
    if (find != string::npos)
        id = std::stoi(headers["url"].substr(find+6));

    map<string, string> params = ProcessTemplatesInDB (templateManager.GetParameterNames(), id);
    
    params["sd"] = string(body.begin(), firstSepPos);
    body = HTTPClient::mergeMapToVector(params);
    output = HTTPClient("localhost", FROM_DB_PORT);
}

void MainProcessDBReceived(map<string, string>& headers, vector<char>& body,
                           map<int, HTTPClient>& pendingDBResponse,
                           std::shared_ptr<std::mutex> pendingDBResponseMutex,
                           HTTPClient& input, HTTPClient& output) {
    map<string, string> bodyParams = HTTPClient::splitVectorToMap(body);

    int sd = std::stoi(bodyParams.at("sd"));
    bodyParams.erase("sd");

    TemplateManager templateManager(headers["url"]);
    body = templateManager.GetHtmlFinal(bodyParams);

    pendingDBResponseMutex->lock();
    output = pendingDBResponse.at(sd);
    pendingDBResponse.erase(sd);
    pendingDBResponseMutex->unlock();
}

void PostProcess(map<string, string>& headers, vector<char>& body, HTTPClient& output) {
    if (headers["proxy"] == "true") {  // Meaning, we need to call another server
        HttpRequestCreator request(headers["http_version"],
                                   HttpRequestCreator::StringToRequestMethod(headers["method"]),
                                   headers["url"],
                                   (headers["Connection"] == "Keep-Alive"),
                                   body);

        if (headers["http_version"] == "1.1" || headers["Connection"] == "Keep-Alive")
            output.send(request.GetRequest(), true);  // will fix later
        else
            output.send(request.GetRequest(), true);
    } else {
        HttpResponse response(headers["http_version"],
                              HttpRequest::StringToRequestMethod(headers["method"]),
                              (headers["Connection"] == "Keep-Alive"),
                              body);

        if (headers["http_version"] == "1.1" || headers["Connection"] == "Keep-Alive")
            output.send(response.GetData(), true);  // will fix later
        else
            output.send(response.GetData(), true);
    }
}

//  {[movietittle,moviedescription,starphoto,starname,movielogo,moviename,videolink,recommended,tittles]}

static map<string, string> ProcessTemplatesInDB(const set<string> &params, size_t ID) {
    map<string, string> result_map;

    vector<string> titles;
    vector<string> stars;
    vector<string> description;
    vector<string> rating;
    vector<string> starphoto;  // not added
    //added double for 0 element skip // fix it later
    titles.push_back("The Boondock Saints");
    stars.push_back("Norman Ridus");
    description.push_back("As saint as the pope!");
    rating.push_back("4");

    titles.push_back("The Boondock Saints");
    stars.push_back("Norman Ridus");
    description.push_back("As saint as the pope!");
    rating.push_back("4");

    titles.push_back("Fight club");
    stars.push_back("Edward Norton");
    description.push_back("You are your biggest enemy.");
    rating.push_back("5");

    titles.push_back("Firefly");
    stars.push_back("Nathan Fillion");
    description.push_back("Season 2 comming never :(");
    rating.push_back("7");

    titles.push_back("Knives out");
    stars.push_back("Daniel Craig");
    description.push_back("Added to watch list");
    rating.push_back("3");

    titles.push_back("Matrix");
    stars.push_back("Keanu Reeves");
    description.push_back("Don't forget to watch it before matrix 4");
    rating.push_back("4");

    titles.push_back("Pirates of the Caribbean");
    stars.push_back("Johny Depp");
    description.push_back("He made it cool to dress up as slutty pirate for halloween");
    rating.push_back("3");

    titles.push_back("Pulp fiction");
    stars.push_back("John Travolta");
    description.push_back("Need to watch it again -.-");
    rating.push_back("5");

    titles.push_back("Radioactive");
    stars.push_back("Imagine Dragons");
    description.push_back("if you don't dance you are no friend of mine");
    rating.push_back("5");

    titles.push_back("Titanic");
    stars.push_back("Leonardo Di Caprio");
    description.push_back("There was enough place for two!");
    rating.push_back("4");

    titles.push_back("True detective");
    stars.push_back("Matthey McConaughey");
    description.push_back("Season one is best series ever. Fight me.");
    rating.push_back("6");

    // vector for randomizing movies (before actual recommendations, based on movie watching experience
    vector<size_t> nums;
    for (size_t i=0; i < titles.size(); i++)
        nums.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(std::begin(nums), std::end(nums), std::default_random_engine(seed));

    // map to actual movie
    result_map["movietittle"] = titles[ID];
    result_map["moviedescription"] = description[ID];
    result_map["starphoto"] = "images/Leo.jpeg";  // replace
    result_map["starname"] = stars[ID];
    result_map["movielogo"] = "/posters/poster_" + std::to_string(ID) + ".jpg";
    result_map["moviename"] = titles[ID];
    result_map["videolink"] = "/video/movie_" + std::to_string(ID) + ".mp4";
    result_map["movierating"] = rating[ID];
    result_map["recommended"] = "6";  // change it in for cycle too

    // map to recommended
    // this cycle adds shuffled parameters, where vector above you can add multiple linked paramets, number has to be same
    // i.e. vector<string> for href and tittle of recommended movies
    for (size_t i = 0; i < 14; i++) {
        size_t k = i % titles.size(); // size + 1, в вектора 1 фильм (0) - лишний
        if (i != ID) {
            result_map["recommended" + std::to_string(i)] = std::to_string(nums[k]);
            result_map["tittles" + std::to_string(i)] = titles[k];
        } else {
            result_map["recommended" + std::to_string(i)] = std::to_string(ID % 10 + 1);
            result_map["tittles" + std::to_string(i)] = titles[ID % 10 + 1];
        }
    }

    return result_map;
}
