#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <pqxx/pqxx>
#include "ports.hpp"

#include "HTTPClient.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpRequestCreator.hpp"
#include "TemplateManager.hpp"
#include "TaskFuncs.hpp"

MainFuncType PreProcess(std::map<std::string, std::string>& headers, std::vector<char>& body, HTTPClient& input) {
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

    if (input.getPort() == FROM_DB_PORT) {
        return MainProcessDBReceived;
    } else if (input.getPort() == TO_DB_PORT) {
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
            body = std::move(templateManager.GetHtmlFinal(std::map<std::string, std::string>()));

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

            output = std::move(HTTPClient("localhost", TO_DB_PORT));
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

static std::map<std::string, std::string> ProcessTemplatesInDB(const std::set<std::string>& params, size_t ID);

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
    std::map<std::string, std::string> params = ProcessTemplatesInDB(templateManager.GetParameterNames(), id);
    
    params["sd"] = std::string(body.begin(), firstSepPos);
    body = std::move(HTTPClient::mergeMapToVector(params));
    output = std::move(HTTPClient("localhost", FROM_DB_PORT));
}

void MainProcessDBReceived(std::map<std::string, std::string>& headers, std::vector<char>& body,
                           std::map<int, HTTPClient>& pendingDBResponse,
                           std::shared_ptr<std::mutex> pendingDBResponseMutex,
                           HTTPClient& input, HTTPClient& output) {
    std::map<std::string, std::string> bodyParams = std::move(HTTPClient::splitVectorToMap(body));

    int sd = std::stoi(bodyParams.at("sd"));
    bodyParams.erase("sd");

    pendingDBResponseMutex->lock();
    output = pendingDBResponse.at(sd);
    pendingDBResponse.erase(sd);
    pendingDBResponseMutex->unlock();

    HttpRequest initialRequest(output.getHeader());
    headers = initialRequest.GetAllHeaders();
    headers["url"] = initialRequest.GetURL();
    headers["method"] = initialRequest.GetRequestMethodString();
    headers["http_version"] = initialRequest.GetHTTPVersion();

    TemplateManager templateManager(headers["url"]);
    body = std::move(templateManager.GetHtmlFinal(bodyParams));
}

void PostProcess(std::map<std::string, std::string>& headers, std::vector<char>& body, HTTPClient& output) {
    if (headers["proxy"] == "true") {  // Meaning, we need to call another server
        HttpRequestCreator request(headers["http_version"],
                                   HttpRequestCreator::StringToRequestMethod(headers["method"]),
                                   headers["url"],
                                   (headers["Connection"] == "Keep-Alive"),
                                   body);
        if ((headers["http_version"] == "1.1" && headers["Conection"] != "close")
            || headers["Connection"] == "Keep-Alive") {
            output.send(request.GetRequest(), true);  // will fix later
        } else {
            output.send(request.GetRequest(), true);
        }

    } else {
        HttpResponse response(headers["http_version"],
                              HttpRequest::StringToRequestMethod(headers["method"]),
                              (headers["Connection"] == "Keep-Alive"),
                              body);
        if ((headers["http_version"] == "1.1" && headers["Conection"] != "close")
            || headers["Connection"] == "Keep-Alive") {
            output.send(response.GetData(), true);  // will fix later
        } else {
            output.send(response.GetData(), true);
        }
    }
}

//  {[movietittle,moviedescription,starphoto,starname,movielogo,moviename,videolink,recommended,tittles]}
static map<string, string> ProcessTemplatesInDB(const set<string> &params, size_t ID) {
    map<string, string> result_map;

    std::string connection_string("host=localhost port=5432 dbname=db_woosh user=vk password=123");

    pqxx::connection con(connection_string.c_str());

    pqxx::work wrk(con);
    //pqxx::result res = wrk.exec("SELECT m_id,title,description,rating FROM MOVIES WHERE (m_id between "+std::to_string(ID)+" and "+std::to_string(ID+1)+")");

    result_map.clear();
    // SELECT for movies-data
    pqxx::result res = wrk.exec("SELECT m_id,title,description,rating,poster,video_link FROM MOVIES WHERE (m_id = "+std::to_string(ID)+")");
    result_map["movietittle"] = res[0][1].as<std::string>();
    result_map["moviename"] = res[0][1].as<std::string>();   // redundant
    result_map["moviedescription"] = res[0][2].as<std::string>();
    result_map["movierating"] = res[0][3].as<std::string>();
    result_map["movielogo"] = res[0][4].as<std::string>();
    result_map["videolink"] = res[0][5].as<std::string>();
    // SELECT for actors
    // Need to figure many to many request for finding actors with m_id == ID from table actors-movies.
    res = wrk.exec("SELECT a_id,name FROM ACTORS WHERE (a_id = "+std::to_string(ID)+")");
    result_map["recommended"] = "6";  // change it in for cycle too
    result_map["starphoto"] = "images/Leo.jpeg";  // replace
    result_map["starname"] = res[0][1].as<std::string>();
    // SELECT for recommended      // crutch (see ID+6) and for +3
    res = wrk.exec("SELECT m_id,title,description,rating FROM MOVIES WHERE (m_id != "+std::to_string(ID)+") ORDER BY RANDOM() LIMIT 10");
    for (size_t i = 0; i < 9; i++) {
        result_map["recommended" + std::to_string(i)] =  res[i][0].as<std::string>();
    }
    res = wrk.exec("SELECT title FROM MOVIES");
    for (size_t i = 0; i < 10; i++) {
        result_map["title" + std::to_string(i)] =  res[i][0].as<std::string>();
    }
    return result_map;
}
