#include "TaskFuncs.hpp"

#include <vector>
#include <map>
#include <string>
#include <fstream>

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
    }

    return MainProcessBasic;
}

void MainProcessBasic(std::map<std::string, std::string>& headers, std::vector<char>& body,
                      HTTPClient& input, HTTPClient& output) {
    ContentType type = HttpResponse::GetContentType(headers["url"]);
    if (type == TXT_HTML) {
        // Template stuff goes here
        // If no db access is required, then output = std::move(input)
        // else, output = HTTPClient(6666, 5) or similar

        // This has to be deleted
        if (headers["url"] == "/")
            headers["url"] = "/index.html";
        std::ifstream source("../static" + headers["url"], std::ios::binary);
        char buffer[BUF_SIZE] = {0};
        while (source.read(buffer, BUF_SIZE)) {
            body.insert(body.end(), buffer, buffer + BUF_SIZE);
        }
        body.insert(body.end(), buffer, buffer + source.gcount());

        output = std::move(input);
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
void MainProcessDBReceived(std::map<std::string, std::string>& headers, std::vector<char>& body,
                           HTTPClient& input, HTTPClient& output) {
    // Get task from "pending response" queue
    // Template postprocesing goes here
    // output = oldTask.GetClient()
}
void PostProcess(std::map<std::string, std::string>& headers, std::vector<char>& body, HTTPClient& output) {
    HttpResponse response(headers["http_version"],
                          HttpRequest::StringToRequestMethod(headers["method"]),
                          headers["url"], headers["Connection"], body);
    // TODO: add "Connection: Keep-Alive" to headers in response
    if (headers["http_version"] == "1.1" || headers["Connection"] == "Keep-Alive") {
        output.send(response.GetData());
    } else {
        output.send(response.GetData(), true);
    }
}