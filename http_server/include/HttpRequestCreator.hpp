#pragma once

#include <map>
#include <string>
#include <vector>

#include "request_methods.hpp"

#define CRLF "\r\n"
#define BUF_SIZE 256

using std::vector;
using std::string;
using std::map;

class HttpRequestCreator {
 public:
    HttpRequestCreator(const string &HTTPVersion, RequestMethod reqType,
                       const string &url, bool keepAlive,
                       const vector<char>& body);

    HttpRequestCreator() = delete;
    string GetHTTPVersion() const;
    string GetHeader() const;
    vector<char> GetRequest() const;

    static string RequestMethodToString(RequestMethod method);
    static RequestMethod StringToRequestMethod(const string& methodString);

 private:
    string http_version;
    RequestMethod reqType;
    string url;
    bool keep_alive;

    vector<char> requestBody;
    map<string, string> headers;
    string requestHeader;
    vector<char> request;

    void FormRequestHeader();
    void SetRequestBody(const vector<char>& body);
    void FormRequestData();
};
