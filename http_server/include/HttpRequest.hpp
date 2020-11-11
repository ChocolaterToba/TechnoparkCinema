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
    std::string GetURL() const {return "/index.html";}
    RequestMethod GetRequestMethod() const {return GET;}
};

