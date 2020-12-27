#pragma once

#include <map>
#include <string>
#include <vector>

#include "request_methods.hpp"

#define CRLF "\r\n"
#define BUF_SIZE 256

using std::string;

class HttpRequestCreator {
 public:
    HttpRequestCreator(const string& HTTPVersion,
                       RequestMethod reqType,
                       const string& url,
                       bool keepAlive,
                       const std::vector<char>& body);

    HttpRequestCreator() = delete;
    string GetHTTPVersion() const;
    string GetHeader() const;
    std::vector<char> GetRequest() const;
    std::map<string, string> GetQueryString() const;
    string GetQueryParameter(const string &parameter_name) const;

    static string RequestMethodToString(RequestMethod method);
    static RequestMethod StringToRequestMethod(const string& methodString);

 private:
    string http_version;
    RequestMethod reqType;
    string url;
    bool keep_alive;

    std::vector<char> requestBody;
    std::map<string, string> headers;
    string requestHeader;
    std::vector<char> request;
    std::map<string, string> query;

    void FormRequestHeader();
    void SetRequestBody(const std::vector<char>& body);
    void FormRequestData();
    void ProcessQueryString();
};
