#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sys/stat.h>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "ResponseBuilder.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"
//#include "exceptions.hpp"  // right now I will need just one
class UnknownContentTypeException: public std::exception {};

void HttpResponse::AddHeader(std::pair<std::string, std::string> header) {
    headers.insert(header);
}

void HttpResponse::SetReturnCode(const std::string &ret_code) {
    return_code = ret_code;
}

std::string HttpResponse::GetReturnCode() const {
    return return_code;
}

bytes HeadOnlyResponse::GetFullData() {
    std::string first_line("HTTP/1.1 " + return_code + CRLF);

    for (auto &header : headers)
        if (!header.second.empty())
            first_line.append(header.first).append(": ").append(header.second).append(CRLF);
    first_line.append(CRLF);

    result.assign(first_line.begin(), first_line.end());
    return result;
}

bytes FullResponse::GetFullData() {
    std::string first_line("HTTP/1.1 " + return_code + CRLF);

    for (auto &header : headers)
        if (!header.second.empty())
            first_line.append(header.first).append(": ").append(header.second).append(CRLF);
    first_line.append(CRLF);

    result.assign(first_line.begin(), first_line.end());
    result.insert(result.end(), body.begin(), body.end());
    return std::move(result);
}

void FullResponse::AddBody(const bytes &src) {
    headers.insert(std::pair<std::string, std::string>("Content-Length", std::to_string(src.size())));
    body = src;
}

std::shared_ptr<HttpResponse> ResponseBuilder::GetResponse() {
    return std::move(response);
}

ResponseBuilder::ResponseBuilder(const HttpRequest &request1) : response(nullptr), request(request1) {}

void StaticResponseBuilder::SetContentType(const std::string &url) {
    ContentType type;
    std::string ext(url.c_str() + url.rfind('.') + 1);
    if (ext == "jpg" || ext == "jpeg" ||
        ext == "JPG" || ext == "JPEG")
        type = IMG_JPG;
    else if (ext == "png" || ext == "PNG")
        type = IMG_PNG;
    else if (ext == "TXT" || ext == "txt")
        type = TXT_PLAIN;
    else if (ext == "html" || ext == "/")
        type = TXT_HTML;
    else if (ext == "css")
        type = TXT_CSS;
    else if (ext == "js")
        type = TXT_JS;
    else
        type = UNDEF;

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
        default:;
            throw UnknownContentTypeException();
    }
    response->AddHeader(c_t_header);
}

bool StaticResponseBuilder::ContentExists(const std::string &url) {
    std::string path("../static" + url);
    if (url == "/")
        path += "index.html";
    return static_cast<bool>(std::ifstream(path, std::ios::binary));
}

bytes StaticResponseBuilder::GetRawData(const std::string &url) {
    bytes data;
    std::string path("../static" + url);
    if (url == "/")
        path += "index.html";

    std::ifstream source(path, std::ios::binary);
    char buffer[BUF_SIZE] = {0};
    while (source.read(buffer, BUF_SIZE)) {
        data.insert(data.end(), buffer, buffer + BUF_SIZE);
    }
    data.insert(data.end(), buffer, buffer + source.gcount());
    return data;
}

void StaticResponseBuilder::MakeResponse() {
    switch (request.GetRequestMethod()) {
        case GET:
            response = std::make_shared<FullResponse>();
            if (ContentExists(request.GetURL()))  // I'm really sorry for the next line :/
                std::dynamic_pointer_cast<std::shared_ptr<FullResponse>>(response)->get()->AddBody(GetRawData(request.GetURL()));
            else
                response->SetReturnCode("404 Not Found");
            break;
        case HEAD:
            response = std::make_shared<HeadOnlyResponse>();
            if (ContentExists(request.GetURL()))
                response->SetReturnCode("204 No Content");
            else
                response->SetReturnCode("404 Not Found");
            break;
        default:
            response->SetReturnCode("501 Not Implemented");
    }
    SetContentType(request.GetURL());
}

StaticResponseBuilder::StaticResponseBuilder(const HttpRequest &request1) : ResponseBuilder(request1) {}

OptionsResponseBuilder::OptionsResponseBuilder(const HttpRequest &request1) : ResponseBuilder(request1) {}

void OptionsResponseBuilder::MakeResponse() {
    response = std::make_shared<HeadOnlyResponse>();
    response->AddHeader(std::pair<std::string, std::string>("Allow", ALLOWED_METHODS));
    response->AddHeader(std::pair<std::string, std::string>("Content-Length", "0"));
    response->SetReturnCode(std::string("200 OK"));
}

VideoResponseBuilder::VideoResponseBuilder(const HttpRequest &request1) : ResponseBuilder(request1) {}

void VideoResponseBuilder::MakeResponse() {
    response = std::make_shared<FullResponse>();
    response->AddHeader(std::pair<std::string, std::string>("Content-type:", "video/mp4"));
    // TODO: Get data from the database and form response, according to the requested piece of bytes
}

AuthorizationHandler_ResponseBuilder::AuthorizationHandler_ResponseBuilder(const HttpRequest &request1) : ResponseBuilder(request1) {}

void AuthorizationHandler_ResponseBuilder::MakeResponse() {
    response = std::make_shared<HeadOnlyResponse>();
    response->SetReturnCode("200 OK");
    // TODO: Make all the logic, that includes parsing the multipart form-data from the request, making needed changes/checks in our user
    //  database, forming corresponding response with confirmation/error giving to the client. However, it will be better to divide
    //  authorization/initialization processes, that have influence on the our database, from the part of code, that just
    //  making response to the client. Or, at least, make it through another method
}
