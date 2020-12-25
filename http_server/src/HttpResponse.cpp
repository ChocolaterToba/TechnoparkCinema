#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <set>
#include <random>
#include <chrono>


#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
#include "exceptions.hpp"


std::string HttpResponse::GetHTTPVersion() const {
    return http_version;
}

std::string HttpResponse::GetHeader() const {
    return response_header;
}
std::vector<char> HttpResponse::GetData() const {
    return response;
}

HttpResponse::HttpResponse(std::string HTTPVersion, RequestMethod reqType,
                           std::string url, bool keepAlive,
                           const std::vector<char>& body): http_version(HTTPVersion), url(url), keep_alive(keepAlive) {
    if (HTTPVersion.empty()) {
        http_version = "0.9";
        if (reqType == GET) {
            try {
                SetResponseBody(body);
                response.insert(response.begin(), response_body.begin(), response_body.end());
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
                return_code = "200 OK";
                SetResponseBody(body);
            }
            catch (HTTPResponseException&) {
                return_code = "404 Not Found";
            }
            break;
        default:
            return_code = "501 Not Implemented";
    }

    FormResponseHeader();
    FormResponseData();
}

void HttpResponse::FormResponseHeader() {
    response_header.clear();
    response_header.append("HTTP/").append(http_version).append(" ");
    response_header.append(return_code).append(" ").append(CRLF);
    
    if (http_version == "1.0" && keep_alive) {
        headers.insert(std::pair<std::string, std::string>("Connection", "Keep-Alive"));
    }
    if (!response_body.empty()) {
        headers.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(response_body.size())));
    }
    for (auto& header_pair : headers) {
        if (!header_pair.second.empty()) {
            response_header.append(header_pair.first).append(": ").append(header_pair.second).append(CRLF);
        }
    }
    response_header.append(CRLF);
}

ContentType HttpResponse::GetContentType(const std::string& url) {
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

void HttpResponse::SetResponseBody(const std::vector<char>& body) {
    response_body.clear();
    response_body.insert(response_body.end(), body.begin(), body.end());
    FormResponseData();
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
    FormResponseHeader();
    FormResponseData();
}

// void HttpResponse::FormResponseHeader() {
//     response_header.clear();
//     response_header.append("HTTP/").append(http_version).append(" ");
//     response_header.append(return_code).append(CRLF);
//     if (http_version == "1.0" && keep_alive == "Keep-Alive") {
//         response_header.append("Connection: Keep-Alive").append(CRLF);
//     }
//     if (!response_body.empty()) {
//         response_header.append("Content-Length: ").append(std::to_string(response_body.size())).append(CRLF);
//     }
//     for (auto& header_pair : headers) {
//         if (!header_pair.second.empty()) {
//             response_header.append(header_pair.first).append(": ").append(header_pair.second).append(CRLF);
//         }
//     }
//     response_header.append(CRLF);
// }

void HttpResponse::FormResponseData() {
    response.assign(response_header.begin(), response_header.end());
    response.insert(response.end(), response_body.begin(), response_body.end());
}
