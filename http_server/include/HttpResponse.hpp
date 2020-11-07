#pragma once

#include "HttpRequest.hpp"

class HttpResponse {
public:
    HttpResponse(const HttpRequest &request);
    std::string GetHTTPVersion() const;
    std::string GetString() const;

private:
    std::string http_version;
    std::string response_string;
    std::string return_code;

    std::string GetHTML(std::string url) const;
};


