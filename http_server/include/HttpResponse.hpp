#pragma once

#include <map>
#include <string>
#include <vector>

#include "content_types.hpp"
#include "request_methods.hpp"

#define CRLF "\r\n"
#define BUF_SIZE 256

using std::vector;
using std::string;


class HttpResponse {
 public:
    HttpResponse(const string &HTTPVersion, RequestMethod reqType, bool keepAlive, const vector<char> &body);

    HttpResponse() = delete;
    string GetHTTPVersion() const;
    string GetHeader() const;
    vector<char> GetData() const;

    static ContentType GetContentType(const string& url);

 private:
    string http_version;
    string return_code;
    bool keep_alive;

    vector<char> response_body;
    std::map<string, string> headers;
    string response_header;
    vector<char> response;

    void SetResponseBody(const vector<char>& body);
    void SetContentType(ContentType type);
    void FormResponseHeader();
    void FormResponseData();
};
