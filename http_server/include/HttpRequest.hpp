#pragma once

// That header just provide me the mocks to write my code properly. However, all these features are implemented by my team in their
// branches.

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
    HttpRequest(std::vector<char>) {}

    virtual std::string GetURL() const {return "index.html";}
    virtual RequestMethod GetRequestMethod() const {return GET;}
};

