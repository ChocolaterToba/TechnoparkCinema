#pragma once

#include <map>
#include <string>
#include <vector>
#include <memory>

#include "HttpRequest.hpp"
//#include "exceptions.hpp"

typedef std::vector<char> bytes;

#define CRLF "\r\n"
#define BUF_SIZE 256

typedef enum {
    UNDEF = -1,
    TXT_PLAIN,
    TXT_HTML,
    TXT_CSS,
    TXT_JS,
    IMG_JPG,
    IMG_PNG,
    VID_MP4,
} ContentType;

class HttpResponse {
protected:
    std::string return_code;
    std::map<std::string, std::string> headers;
    bytes result;
public:
    virtual bytes GetFullData() = 0;
    void AddHeader(std::pair<std::string, std::string> header);
    void SetReturnCode(const std::string &ret_code);
    std::string GetReturnCode() const;
};

class HeadOnlyResponse: public HttpResponse {
public:
    bytes GetFullData() override;
};

class FullResponse: public HttpResponse {
private:
    bytes body;
public:
    bytes GetFullData() override;
    void AddBody(const bytes &src);
};
