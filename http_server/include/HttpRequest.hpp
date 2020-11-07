#pragma once

#include <vector>
#include <map>
#include <string>

typedef enum {
    UNKNOWN = -1,
    GET,
    POST,
    OPTIONS,
    HEAD,
    PUT,
    PATCH,
    DELETE,
    CONNECT,
} RequestMethod;

class HttpRequest {
 public:
    HttpRequest(const std::string &message);

    std::string GetHeader(std::string &header_name) const;
    std::string GetURL() const;
    std::string GetHTTPVersion() const;
    RequestMethod GetRequestMethod() const;

private:
    RequestMethod request_method;
    std::string url;
    std::string http_version;
    std::map<std::string, std::string> headers;

    void GetRequestMethod(const std::string &method_name);
};
