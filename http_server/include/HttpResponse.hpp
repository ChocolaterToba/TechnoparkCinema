#pragma once

#include <map>
#include <string>

#include "HttpRequest.hpp"

#define CRLF "\r\n"

class HttpResponse {
public:
    explicit HttpResponse(const HttpRequest &request);
    std::string GetHTTPVersion() const;
    std::string GetString() const;

private:
    std::string http_version;
    std::string response_string;
    std::string return_code;
    std::string html_file;

    std::map<std::string, std::string> headers;

    std::string GetHTML(const std::string &url) const;
    void FormResponseString();
};


