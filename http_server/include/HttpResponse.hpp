#pragma once

#include <map>
#include <string>

#include "HttpRequest.hpp"

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

typedef struct {
    std::string data_string;
    ContentType data_type;
} Data;

class HttpResponse {
public:
    explicit HttpResponse(const HttpRequest &request);
//    HttpResponse() = default;  //
    std::string GetHTTPVersion() const;
    std::string GetString() const;

private:
    std::string http_version;
    std::string response_string;
    std::string return_code;
    Data data;

    std::map<std::string, std::string> headers;

    void SetData(const std::string &url);
    ContentType GetContentType(const std::string &url) const;
    void SetContentType(ContentType type);
    void FormResponseString();
};


