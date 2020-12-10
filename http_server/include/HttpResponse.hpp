#pragma once

#include <map>
#include <string>
#include <vector>

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

class HttpResponse {
 public:
    explicit HttpResponse(const HttpRequest &request);
    HttpResponse() = default;
    std::string GetHTTPVersion() const;
    std::vector<char> GetData() const;

 private:
    std::string http_version;
    std::vector<char> response_data;
    std::string return_code;

    std::vector<char> data;

    std::map<std::string, std::string> headers;

    void SetData(const std::string &url);
    ContentType GetContentType(const std::string &url) const;
    void SetContentType(ContentType type);
    void FormResponseData();
};
