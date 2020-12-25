#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <random>

#include "TaskFuncs.hpp"
#include "HTTPClient.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "TemplateManager.hpp"

MainFuncType PreProcess(std::map<std::string, std::string>& headers, std::vector<char>& body, HTTPClient& input) {
    input.recvHeader();
    HttpRequest request(input.getHeader());
    int bodySize = request.GetContentLength();

    headers.clear();
    headers = request.GetAllHeaders();
    headers["url"] = request.GetURL();
    headers["method"] = request.GetRequestMethodString();
    headers["http_version"] = request.GetHTTPVersion();

    body.clear();
    if (bodySize > 0) {
        input.recvBody(bodySize);
        body = input.getBody();
    }

    if (input.getPort() == 6666) {
        return MainProcessDBReceived;
    } else if (input.getPort() == 7777) {
        return MainProcessDBServer;
    }

    return MainProcessBasic;
}

void MainProcessBasic(std::map<std::string, std::string>& headers, std::vector<char>& body,
                      std::map<int, HTTPClient>& pendingDBResponse,
                      std::shared_ptr<std::mutex> pendingDBResponseMutex,
                      HTTPClient& input, HTTPClient& output) {
    ContentType type = HttpResponse::GetContentType(headers["url"]);
    if (type == TXT_HTML) {
        TemplateManager templateManager(headers["url"]);
        std::set<std::string> params = std::move(templateManager.GetParameterNames());
        if (params.empty()) {
            body = templateManager.GetHtmlFinal(std::map<std::string, std::string>());

            output = std::move(input);
        } else {
            pendingDBResponseMutex->lock();
            pendingDBResponse.insert(std::pair<int, HTTPClient&>(input.getSd(), input));
            pendingDBResponseMutex->unlock();

            headers["Connection"] = "close";  // maybe make headers from scratch???

            body.clear();
            std::string sdString = std::to_string(input.getSd());
            body.insert(body.end(), sdString.begin(), sdString.end());
            body.push_back('|');
            std::vector<char> paramsPart = std::move(HTTPClient::mergeSetToVector(params));
            body.insert(body.end(), paramsPart.begin(), paramsPart.end());

            output = HTTPClient("localhost", 7777);
            headers["proxy"] = "true";
        }
    } else {
        std::ifstream source("../static" + headers["url"], std::ios::binary);
        char buffer[BUF_SIZE] = {0};
        while (source.read(buffer, BUF_SIZE)) {
            body.insert(body.end(), buffer, buffer + BUF_SIZE);
        }
        body.insert(body.end(), buffer, buffer + source.gcount());

        output = std::move(input);
    }
}
static std::map<std::string, std::string> ProcessTemplatesInDB(std::set<std::string> params, size_t ID);
void MainProcessDBServer(std::map<std::string, std::string>& headers, std::vector<char>& body,
                         std::map<int, HTTPClient>& pendingDBResponse,
                         std::shared_ptr<std::mutex> pendingDBResponseMutex,
                         HTTPClient& input, HTTPClient& output) {
    auto firstSepPos = std::find(body.begin(), body.end(), '|');
    if (firstSepPos > body.end()) {
        firstSepPos = body.end();
    }
    TemplateManager templateManager(headers["url"]);
    size_t id = 0;
    size_t find = headers["url"].find("watch?");
    if (find != std::string::npos) {
        id = std::stoi(headers["url"].substr(find+6));
    }
    std::map<std::string, std::string> params = ProcessTemplatesInDB (templateManager.GetParameterNames(), id);
    
    params["sd"] = std::string(body.begin(), firstSepPos);
    body = std::move(HTTPClient::mergeMapToVector(params));
    output = HTTPClient("localhost", 6666);
}

void MainProcessDBReceived(std::map<std::string, std::string>& headers, std::vector<char>& body,
                           std::map<int, HTTPClient>& pendingDBResponse,
                           std::shared_ptr<std::mutex> pendingDBResponseMutex,
                           HTTPClient& input, HTTPClient& output) {
    std::map<std::string, std::string> bodyParams = std::move(HTTPClient::splitVectorToMap(body));

    int sd = std::stoi(bodyParams.at("sd"));
    bodyParams.erase("sd");

    TemplateManager templateManager(headers["url"]);
    body = std::move(templateManager.GetHtmlFinal(bodyParams));

    pendingDBResponseMutex->lock();
    output = pendingDBResponse.at(sd);
    pendingDBResponse.erase(sd);
    pendingDBResponseMutex->unlock();
}

void PostProcess(std::map<std::string, std::string>& headers, std::vector<char>& body, HTTPClient& output) {
    bool proxy = false;
    if (headers["proxy"] == "true") {  // Meaning, we need to call another server
        proxy = true;
    }
    HttpResponse response(headers["http_version"],
                          HttpRequest::StringToRequestMethod(headers["method"]),
                          headers["url"], headers["Connection"], body, proxy);

    if (headers["http_version"] == "1.1" || headers["Connection"] == "Keep-Alive") {
        std::cout << "keep-alive" << std::endl;
        output.send(response.GetData(), true);
    } else {
        output.send(response.GetData(), true);
    }
    std::cout << "done" << std::endl;
}

//  {[movietittle,moviedescription,starphoto,starname,movielogo,moviename,videolink,recommended,tittles]}
static std::map<std::string, std::string> ProcessTemplatesInDB(std::set<std::string> params, size_t ID) {
    std::map<std::string, std::string> result_map;
    // vector for randomizing movies (before actual recommendations, based on movie watching experience
    std::vector<size_t> nums;
    for (size_t i=0; i < 10; i++)
        nums.push_back(i);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(std::begin(nums), std::end(nums), std::default_random_engine(seed));

    std::vector<std::string> tittles;
    std::vector<std::string> stars;
    std::vector<std::string> description;
    std::vector<std::string> rating;
    std::vector<std::string> starphoto;  // not added
    //added double for 0 element skip // fix it later
    tittles.push_back("The Boondock Saints");
    stars.push_back("Norman Ridus");
    description.push_back("As saint as the pope!");
    rating.push_back("4");

    tittles.push_back("The Boondock Saints");
    stars.push_back("Norman Ridus");
    description.push_back("As saint as the pope!");
    rating.push_back("4");

    tittles.push_back("Fight club");
    stars.push_back("Edward Norton");
    description.push_back("You are your biggest enemy.");
    rating.push_back("5");

    tittles.push_back("Firefly");
    stars.push_back("Nathan Fillion");
    description.push_back("Season 2 comming never :(");
    rating.push_back("7");

    tittles.push_back("Knives out");
    stars.push_back("Daniel Craig");
    description.push_back("Added to watch list");
    rating.push_back("3");

    tittles.push_back("Matrix");
    stars.push_back("Keanu Reeves");
    description.push_back("Don't forget to watch it before matrix 4");
    rating.push_back("4");

    tittles.push_back("Pirates of the Caribbean");
    stars.push_back("Johny Depp");
    description.push_back("He made it cool to dress up as slutty pirate for halloween");
    rating.push_back("3");

    tittles.push_back("Pulp fiction");
    stars.push_back("John Travolta");
    description.push_back("Need to watch it again -.-");
    rating.push_back("5");

    tittles.push_back("Radioactive");
    stars.push_back("Imagine Dragons");
    description.push_back("if you don't dance you are no friend of mine");
    rating.push_back("5");

    tittles.push_back("Titanic");
    stars.push_back("Leonardo Di Caprio");
    description.push_back("There was enough place for two!");
    rating.push_back("4");

    tittles.push_back("True detective");
    stars.push_back("Matthey McConaughey");
    description.push_back("Season one is best series ever. Fight me.");
    rating.push_back("6");


    // map to actual movie
    result_map["movietittle"] = tittles[ID];
    result_map["moviedescription"] = description[ID];
    result_map["starphoto"] = "images/Leo.jpeg";  // replace
    result_map["starname"] = stars[ID];
    result_map["movielogo"] = "/posters/poster_" + std::to_string(ID) + ".jpg";
    result_map["moviename"] = tittles[ID];
    result_map["videolink"] = "/video/movie_" + std::to_string(ID) + ".mp4";
    result_map["movierating"] = rating[ID];
    result_map["recommended"] = "6";  // change it in for cycle too

    // map to recommended
    // this cycle adds shuffled parameters, where vector above you can add multiple linked paramets, number has to be same
    // i.e. vector<string> for href and tittle of recommended movies
    for (size_t i = 0; i < 14; i++) {
        size_t k = i % 10;
        if (i != ID) {
            result_map["recommended" + std::to_string(i)] = std::to_string(nums[k]);
            result_map["tittles" + std::to_string(i)] = tittles[k];
        } else {
            result_map["recommended" + std::to_string(i)] = std::to_string(ID % 10 + 1);
            result_map["tittles" + std::to_string(i)] = tittles[ID % 10 + 1];
        }
    }

    return result_map;
}
