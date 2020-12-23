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
#include "DatabaseManager.hpp"
#include "TemplateManager.hpp"
#include "exceptions.hpp"


std::string HttpResponse::GetHTTPVersion() const {
    return http_version;
}

std::vector<char> HttpResponse::GetData() const {
    return std::move(response_data);
}

HttpResponse::HttpResponse(const HttpRequest &request): http_version("1.1") {
    if (request.GetHTTPVersion().empty()) {
        http_version = "0.9";
        if (request.GetRequestMethod() == GET) {
            try {
                SetData(request.GetURL());
                std::copy(response_data.begin(), response_data.end(), data.begin());
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

    response_data.assign(answer.begin(), answer.end());
    response_data.insert(response_data.end(), data.begin(), data.end());
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


void HttpResponse::SetData(const std::string &url) {
    std::vector<std::string> vect;
    vect.push_back("images/img6.jpg");
    vect.push_back("images/img5.jpg");
    vect.push_back("images/img4.jpg");
    vect.push_back("images/img3.jpg");
    vect.push_back("images/img2.jpg");
    vect.push_back("images/img1.jpg");
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(std::begin(vect), std::end(vect), std::default_random_engine(seed));


    SetContentType(GetContentType(url));
    std::string path("../static" + url);
    if (url == "/") 
        path += "IndexTemplate.html";  // "index.html"


    TemplateManager TManager(url);

    std::map<std::string, std::string> params; 
    params["movietittle"] = "Titanic";
    params["moviedescription"] = "Subscribe Woosh.com to watch more kittens.";
    params["starphoto"] = "images/Leo.jpeg";
    params["starname"] = "Leonardo Dicaprio";
    params["movielogo"] = "images/img1.jpg";
    params["moviename"] = "Titanic";
    params["videolink"] = "lorem_ipsum.mp4";
    params["movierating"] = "3";
    params["recommended"] = std::to_string(vect.size());
    for (size_t i = 0; i < vect.size(); i++)
        params["recommended" + std::to_string(i)] = vect[i];

    std::vector<char> result = TManager.GetHtmlFinal(params);
    data.insert(data.end(), result.begin(), result.end());
}
