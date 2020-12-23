#include <vector>
#include <map>
#include <string>
#include <fstream>

#include "TaskFuncs.hpp"
#include "HTTPClient.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

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
        input.recvBody(bodySize); // is not necessary right now
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
        // Template stuff goes here
        // If no db access is required, then
        // output = std::move(input);
        // else
        pendingDBResponseMutex->lock();
        pendingDBResponse.insert(std::pair<int, HTTPClient&>(input.getSd(), input));
        pendingDBResponseMutex->unlock();

        headers["Connection"] = "close";  // maybe make headers from scratch???

        std::queue<std::string> bodyParams;
        bodyParams.push(std::to_string(input.getSd()));
        // add other parameters to bodyParams here
        body = HTTPClient::mergeQueueToVector(bodyParams);

        output = HTTPClient("localhost", 7777);
        
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
    // Replace this with actual body parameter handling
    body.push_back('|');

    std::ifstream source("../static/index.html", std::ios::binary);
    char buffer[BUF_SIZE] = {0};
    while (source.read(buffer, BUF_SIZE)) {
        body.insert(body.end(), buffer, buffer + BUF_SIZE);
    }
    body.insert(body.end(), buffer, buffer + source.gcount());

    output = HTTPClient("localhost", 6666);
}
void MainProcessDBReceived(std::map<std::string, std::string>& headers, std::vector<char>& body,
                           std::map<int, HTTPClient>& pendingDBResponse,
                           std::shared_ptr<std::mutex> pendingDBResponseMutex,
                           HTTPClient& input, HTTPClient& output) {
    std::queue<std::string> bodyParams = HTTPClient::splitVectorToQueue(body);
    int sd = std::stoi(bodyParams.front());
    bodyParams.pop();
    output = pendingDBResponse.at(sd);  //  thread-safe
    pendingDBResponseMutex->lock();
    pendingDBResponse.erase(sd);
    pendingDBResponseMutex->unlock();
    // Template postprocesing goes here
    // headers are taken from pending client as if no DB accessing happened
    // body is resulting document

    // delete this later
    body = HTTPClient::mergeQueueToVector(bodyParams);
}
void PostProcess(std::map<std::string, std::string>& headers, std::vector<char>& body, HTTPClient& output) {
    HttpResponse response(headers["http_version"],
                          HttpRequest::StringToRequestMethod(headers["method"]),
                          headers["url"], headers["Connection"], body);

    if (headers["http_version"] == "1.1" || headers["Connection"] == "Keep-Alive") {
        output.send(response.GetData());
    } else {
        output.send(response.GetData(), true);
    }
}