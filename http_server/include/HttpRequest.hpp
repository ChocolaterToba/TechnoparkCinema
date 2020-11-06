#pragma once

#include <vector>
#include <map>
#include <string>

class HttpMessage {
 public:
    virtual std::string Host();
    std::string User_Agent();
 protected:
    std::vector<std::string> headers;
};

class HttpResponse: public HttpMessage {

};

class HttpRequest: public HttpMessage {
public:
    HttpRequest(const std::string &message);



};
