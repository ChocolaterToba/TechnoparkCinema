#pragma once

#include <map>
#include <string>
#include <vector>

#include "request_methods.hpp"

#define CRLF "\r\n"
#define BUF_SIZE 256

class HttpRequestCreator {
 public:
    HttpRequestCreator(const std::string& HTTPVersion,
                       RequestMethod reqType,
                       const std::string& url,
                       bool keepAlive,
                       const std::vector<char>& body);

    HttpRequestCreator() = delete;
    std::string GetHTTPVersion() const;
    std::string GetHeader() const;
    std::vector<char> GetRequest() const;

    static std::string RequestMethodToString(RequestMethod method);
    static RequestMethod StringToRequestMethod(const std::string& methodString);

 private:
    std::string http_version;
    RequestMethod reqType;
    std::string url;
    bool keep_alive;

    std::vector<char> requestBody;
    std::map<std::string, std::string> headers;
    std::string requestHeader;
    std::vector<char> request;

    void FormRequestHeader();
    void SetRequestBody(const std::vector<char>& body);
    void FormRequestData();
};
