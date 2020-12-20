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
    explicit HttpResponse::HttpResponse(std::string HTTPVersion, RequestMethod reqType, std::string url, std::vector<char> body);
    HttpResponse() = default;
    std::string GetHTTPVersion() const;
    std::vector<char> GetData() const;

    static ContentType GetContentType(const std::string& url);

private:
    std::string http_version;
    std::vector<char> response;
    std::string return_code;

    std::vector<char> response_body;

    std::map<std::string, std::string> headers;

    void SetResponseBody(const std::vector<char>& body);
    void SetContentType(ContentType type);
    void FormResponseData();
};
