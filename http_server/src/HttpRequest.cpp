#include <exception>

#include "HttpRequest.hpp"

std::string HttpRequest::GetHeader(std::string &string) const {
    auto iter = headers.find(string);
    if (iter == headers.end()) {
        return std::string();
    } else {
        return iter->second;
    }
}

HttpRequest::HttpRequest(const std::string &message) {
    url = "/index";
    http_version = "1.1";
    request_method = GET;
}

void HttpRequest::GetRequestMethod(const std::string &method_name) {
    if (method_name == "GET")
        request_method = GET;
    else if (method_name == "POST")
        request_method = POST;
    else if (method_name == "OPTIONS")
        request_method = OPTIONS;
    else if (method_name == "HEAD")
        request_method = HEAD;
    else if (method_name == "PUT")
        request_method = PUT;
    else if (method_name == "PATCH")
        request_method = PATCH;
    else if (method_name == "DELETE")
        request_method = DELETE;
    else if (method_name == "CONNECT")
        request_method = CONNECT;
    else
        request_method = UNKNOWN;
}

std::string HttpRequest::GetURL() const {
    return url;
}

std::string HttpRequest::GetHTTPVersion() const {
    return http_version;
}

RequestMethod HttpRequest::GetRequestMethod() const {
    return request_method;
}
