#include <string>
#include <map>
#include <vector>

#include "exceptions.hpp"
#include "HttpRequest.hpp"

HttpRequest::HttpRequest(const std::string &message) {
    // find Method
    size_t pos = 0;
    size_t search = message.find(' ');
    std::string method = message.substr(pos, search);
    CheckRequestMethod(method);
    if (request_method == UNKNOWN) {
        throw BadFormatException();
    }
    // find URL
    search++;
    pos = search;
    while ((message[search] != ' ') && (message[search] != '\r')) {
        search++;
    }
    if (search == pos) {
        throw BadFormatException();
    }
    url = message.substr(pos, search-pos);
    // find HTTP version
    search++;
    pos = 0;
    if ((pos = message.find("HTTP/", search - 1, 5)) != std::string::npos) {
        pos = pos+5;
        search = pos;
        while (message[search] != '\r') {
            search++;
        }
        http_version = message.substr(pos, search-pos);
    } else {
        http_version = std::string("");   // check
    }
    search+=2;   // \r\n 2 symbols

    // get headers
    pos = message.find(": ", search);
    while (pos != std::string::npos) {
        std::string key = message.substr(search, pos-search);
        pos+=2;
        search = pos;
        while (message[search] != '\r') {
            search++;
        }
        std::string value = message.substr(pos, search-pos);
        headers.insert(std::pair<std::string, std::string>(key, value));
        search += 2;  // \r\n 2 symbols
        pos = message.find(": ", search);
    }
}

std::string HttpRequest::GetHeader(std::string &string) const {
    auto iter = headers.find(string);
    if (iter == headers.end()) {
        return std::string();
    } else {
        return iter->second;
    }
}

void HttpRequest::CheckRequestMethod(const std::string &method_name) {
    if (method_name == "GET")
        request_method = GET;
    else if (method_name == "POST")
        request_method = POST;
    else if (method_name == "OPTIONS")
        request_method = OPTIONS;
    else if (method_name == "HEAD")
        request_method = HEAD;
    else if (method_name == "PUT")
        request_method = PUT;
    else if (method_name == "PATCH")
        request_method = PATCH;
    else if (method_name == "DELETE")
        request_method = DELETE;
    else if (method_name == "CONNECT")
        request_method = CONNECT;
    else
        request_method = UNKNOWN;
}

std::string HttpRequest::GetURL() const {
    return url;
}

std::string HttpRequest::GetHTTPVersion() const {
    return http_version;
}

RequestMethod HttpRequest::GetRequestMethod() const {
    return request_method;
}

void HttpRequest::SetBody(std::string new_body) {
    // body = new_body;
    std::string boundary = HttpRequest::GetBoundary();
    if (boundary.empty())
        throw BadFormatException();
    boundary = std::string("--") + boundary;
    size_t search = new_body.find(boundary);
    size_t pos = 0;
    while (search != std::string::npos) {
        pos = search  + boundary.length() + 2;
        search = new_body.find(boundary, pos);
        if (search != std::string::npos)
            body_parts.push_back(new_body.substr(pos, search-pos));
    }
    if (pos == 0) {
        body_parts.push_back(new_body);
    }
    size_t len = new_body.length();
    if (pos < len) {
        body_parts.push_back(new_body.substr(pos, new_body.length() - pos));
    }
}


std::string HttpRequest::GetBoundary() const {
    auto iter = headers.find(std::string("Content-Type"));
    if (iter == headers.end()) {
        return std::string();
    } else {
        size_t pos = iter->second.find(std::string("boundary="));
        size_t len = (std::string("boundary=")).length();
        std::string boundary = iter->second.substr(pos + len, iter->second.length() - pos);
        return boundary;
    }
}

size_t HttpRequest::GetContentLen() const {
    auto iter = headers.find(std::string("Content-Length"));
    if (iter == headers.end()) {
        return 0;
    } else {
        return stoi(iter->second);
    }
}
