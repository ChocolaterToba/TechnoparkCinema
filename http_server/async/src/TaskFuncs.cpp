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
            headers["flag"] = "value";
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

void MainProcessDBServer(std::map<std::string, std::string>& headers, std::vector<char>& body,
                         std::map<int, HTTPClient>& pendingDBResponse,
                         std::shared_ptr<std::mutex> pendingDBResponseMutex,
                         HTTPClient& input, HTTPClient& output) {
    auto firstSepPos = std::find(body.begin(), body.end(), '|');
    if (firstSepPos > body.end()) {
        firstSepPos = body.end();
    }
    TemplateManager templateManager(headers["url"]);

    // vector for randomizing movies (before actual recommendations, based on movie watching experience
    std::vector<std::string> vect;
    vect.push_back("images/img6.jpg");
    vect.push_back("images/img5.jpg");
    vect.push_back("images/img4.jpg");
    vect.push_back("images/img3.jpg");
    vect.push_back("images/img2.jpg");
    vect.push_back("images/img1.jpg");
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(std::begin(vect), std::end(vect), std::default_random_engine(seed));

    // map of actual parameters
    std::map<std::string, std::string> params; 
    params["movietittle"] = "Titanic";
    params["moviedescription"] = "Subscribe Woosh.com to watch more kittens.";
    params["starphoto"] = "images/Leo.jpeg";
    params["starname"] = "Leonardo Dicaprio";
    params["movielogo"] = "images/img1.jpg";
    params["moviename"] = "Titanic";
    params["videolink"] = "lorem_ipsum.mp4";
    params["movierating"] = "3";
    params["recommended"] = std::to_string(vect.size());
    
    // this cycle adds shuffled parameters, where vector above you can add multiple linked paramets, number has to be same
    // i.e. vector<string> for href and tittle of recommended movies 
    for (size_t i = 0; i < vect.size(); i++)
        params["recommended" + std::to_string(i)] = vect[i];
    
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
    bool flag = false;
    if (headers["flag"] == "value") {
        flag = true;
    }
    HttpResponse response(headers["http_version"],
                          HttpRequest::StringToRequestMethod(headers["method"]),
                          headers["url"], headers["Connection"], body, flag);

    if (headers["http_version"] == "1.1" || headers["Connection"] == "Keep-Alive") {
        output.send(response.GetData());
    } else {
        output.send(response.GetData(), true);
    }
}