#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "exceptions.hpp"


std::string HttpResponse::GetHTTPVersion() const {
    return http_version;
}

std::vector<char> HttpResponse::GetData() const {
    return std::move(response);
}
    // std::string GetURL() const;
    // std::string GetHTTPVersion() const;
    // RequestMethod GetRequestMethod() const;
HttpResponse::HttpResponse(std::string HTTPVersion, RequestMethod reqType, std::string url, std::vector<char> body): http_version("1.1") {
    if (HTTPVersion.empty()) {
        http_version = "0.9";
        if (reqType == GET) {
            try {
                SetResponseBody(body);
                std::copy(response.begin(), response.end(), response_body.begin());
                return;
            }
            catch (FileNotFoundException &) {
                throw OldVersionException();
            }
        } else {
            throw OldVersionException();
        }
    }

    switch (reqType) {
        case GET:
            try {
                SetResponseBody(body);
                return_code = "200 OK";
            }
            catch (HTTPResponseException&) {
                return_code = "404 Not Found";
            }
            break;
        default:
            return_code = "501 Not Implemented";
    }

    FormResponseData();
}


void HttpResponse::FormResponseData() {
    std::string answer("");
    answer.append("HTTP/").append(http_version).append(" ");
    answer.append(return_code).append(CRLF);
    for (auto &header : headers) {
        if (!header.second.empty()) {
            answer.append(header.first).append(": ").append(header.second).append(CRLF);
        }
    }
    answer.append(CRLF);

    response.assign(answer.begin(), answer.end());
    response.insert(response.end(), response_body.begin(), response_body.end());
}

ContentType HttpResponse::GetContentType(const std::string &url) const {
    std::string ext(url.c_str() + url.rfind('.') + 1);
    if (ext == "/") {
        return TXT_HTML;
    }
    if (ext == "jpg" || ext == "jpeg" ||
        ext == "JPG" || ext == "JPEG")
        return IMG_JPG;
    else if (ext == "png" || ext == "PNG")
        return IMG_PNG;
    else if (ext == "TXT" || ext == "txt")
        return TXT_PLAIN;
    else if (ext == "html")
        return TXT_HTML;
    else if (ext == "css")
        return TXT_CSS;
    else if (ext == "js")
        return TXT_JS;
    else if (ext == "mp4")
        return VID_MP4;
    else
        return UNDEF;
}

void HttpResponse::SetContentType(ContentType type) {
    std::pair<std::string, std::string> c_t_header;
    c_t_header.first = "Content-type";
    switch (type) {
        case TXT_HTML:
            c_t_header.second = "text/html";
            break;
        case TXT_CSS:
            c_t_header.second = "text/css";
            break;
        case TXT_JS:
            c_t_header.second = "text/javascript";
            break;
        case TXT_PLAIN:
            c_t_header.second = "text/plain";
            break;
        case IMG_JPG:
            c_t_header.second = "image/jpeg";
            break;
        case IMG_PNG:
            c_t_header.second = "image/png";
            break;
        case VID_MP4:
            c_t_header.second = "video/mp4";
            break;
        default:
            throw UnknownContentTypeException();
    }

    headers.insert(c_t_header);
}


void HttpResponse::SetResponseBody(const std::vector<char> body) {
    response_body.insert(response_body.end(), body.begin(), body.end());
    // OLD
    // SetContentType(GetContentType(url));
    // std::string path("../static" + url);
    // if (url == "/")
    //     path += "index.html";

    // std::ifstream source(path, std::ios::binary);
    // char buffer[BUF_SIZE] = {0};
    // while (source.read(buffer, BUF_SIZE)) {
    //     response_body.insert(response_body.end(), buffer, buffer + BUF_SIZE);
    // }
    // response_body.insert(response_body.end(), buffer, buffer + source.gcount());
}
