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
    HttpResponse(std::string HTTPVersion, RequestMethod reqType,
                 std::string url, std::string keepAlive,
                 std::vector<char> body);
    HttpResponse() = default;
    std::string GetHTTPVersion() const;
    std::string GetHeader() const;
    std::vector<char> GetData() const;

    static ContentType GetContentType(const std::string& url);

private:
    std::string http_version;
    std::string return_code;
    std::string url;
    std::string keep_alive;

    std::vector<char> response_body;
    std::map<std::string, std::string> headers;
    std::string response_header;
    std::vector<char> response;

    void SetResponseBody(const std::vector<char>& body);
    void SetContentType(ContentType type);
    void FormResponseHeader();
    void FormResponseData();
};
