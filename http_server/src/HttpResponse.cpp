#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>

#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "exceptions.hpp"


std::string HttpResponse::GetHTTPVersion() const {
    return http_version;
}

std::string HttpResponse::GetString() const {
    return response_string;
}

HttpResponse::HttpResponse(const HttpRequest &request) {
    response_string = "";
    if (request.GetHTTPVersion().empty()) {
        http_version = "0.9";
        if (request.GetRequestMethod() == GET) {
            try {
                SetData(request.GetURL());
                response_string = data.data_string;
                return;
            }
            catch (FileNotFoundException &) {
                throw OldVersionException();
            }
        } else {
            throw OldVersionException();
        }
    }

    switch (request.GetRequestMethod()) {
        case GET:
            try {
                SetData(request.GetURL());
                return_code = "200 OK";
                http_version = "1.1";
            }
            catch (FileNotFoundException&) {
                return_code = "404 Not Found";
                data.data_string = "";
                http_version = "";
            }
            break;
        default:
            return_code = "501 Not Implemented";
    }

    FormResponseString();
}


void HttpResponse::FormResponseString() {
    response_string.append("HTTP/").append(http_version).append(" ");
    response_string.append(return_code).append(CRLF);
    for (auto &header : headers) {
        response_string.append(header.first).append(": ").append(header.second).append(CRLF);
    }
    if (!data.data_string.empty())
        response_string.append(CRLF).append(data.data_string).append(CRLF);
}

ContentType HttpResponse::GetContentType(const std::string &url) const {
    std::string ext(url.c_str() + url.rfind('.') + 1);
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
            c_t_header.second = "text/jpeg";
            break;
        case IMG_PNG:
            c_t_header.second = "text/png";
            break;
        case VID_MP4:
            c_t_header.second = "text/mp4";
            break;
        default:
            throw UnknownContentTypeException();
    }

    headers.insert(c_t_header);
}


void HttpResponse::SetData(const std::string &url) {
    SetContentType(GetContentType(url));
    std::string &str = data.data_string;
    std::ifstream source(url.c_str() + 1, std::ios::in | std::ios::binary);
    char buffer[BUF_SIZE] = {0};
    while (source) {
        source.read(buffer, BUF_SIZE);
        str.append(buffer);
        memset(buffer, '\0', BUF_SIZE);
    }
}
