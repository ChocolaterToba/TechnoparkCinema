#include <string>
#include <map>

#include "exceptions.hpp"
#include "HttpRequest.hpp"

HttpRequest::HttpRequest(const string &message) {
    // find Method
    size_t pos = 0;
    size_t search = message.find(' ');
    string method = message.substr(pos, search);
    SetRequestMethod(method);
    if (request_method == UNKNOWN)  // throw BadFormatException();  // temporary
        SetRequestMethod("GET");

    // find URL
    ++search;
    pos = search;
    while ((message[search] != ' ') && (message[search] != '\r'))
        search++;

    if (search == pos)
        throw BadFormatException();

    url = message.substr(pos, search-pos);
    if (url.rfind('/', 0) != 0) {  // temporary
        url = "/";
        search = 0;
    }

    // find HTTP version
    ++search;
    pos = 0;
    if ((pos = message.find("HTTP/", search - 1, 5)) != string::npos) {
        pos = pos+5;
        search = pos;
        while (message[search] != '\r' && message[search] != ' ')
            ++search;

        http_version = message.substr(pos, search-pos);
    } else {
        http_version = string("");   // check
    }

    while (message[search] != '\r')  //  temporary, skipping response code
        ++search;

    search += 2;   // \r\n 2 symbols
    // get headers
    while ((pos = message.find(": ", search)) != string::npos) {
        string key = message.substr(search, pos-search);
        pos += 2;
        search = pos;
        while (message[search] != '\r')
            ++search;

        string value = message.substr(pos, search-pos);
        headers.insert (std::pair<string, string>(key, value));
        search += 2;  // \r\n 2 symbols
    }
}

string HttpRequest::GetHeader(const std::basic_string<char> &header_name) const {
    auto iter = headers.find(header_name);
    if (iter == headers.end())
        return string();
    else
        return iter->second;
}

void HttpRequest::SetRequestMethod(const string& method_name) {
    request_method = StringToRequestMethod(method_name);
}

string HttpRequest::GetURL() const {
    return url;
}

string HttpRequest::GetHTTPVersion() const {
    return http_version;
}

RequestMethod HttpRequest::GetRequestMethod() const {
    return request_method;
}

string HttpRequest::GetRequestMethodString() const {
    return RequestMethodToString(request_method);
}

map<string, string> HttpRequest::GetAllHeaders() const {
    return headers;
}

int HttpRequest::GetContentLength() {
    int result;
    try {
        result = std::stoi(headers["Content-Length"]);
    }
    catch (std::invalid_argument &e) {
        result = -1;
    }
    return result;
}

string HttpRequest::RequestMethodToString(RequestMethod method) {
    switch (method) {
        case GET:
            return "GET";
        case POST:
            return "POST";
        case OPTIONS:
            return "OPTIONS";
        case HEAD:
            return "HEAD";
        case PUT:
            return "PUT";
        case PATCH:
            return "PATCH";
        case DELETE:
            return "DELETE";
        case CONNECT:
            return "CONNECT";
        default:
            return "UNKNOWN";
    }
}

RequestMethod HttpRequest::StringToRequestMethod(const string& methodString) {
    RequestMethod method;
    if (methodString == "GET")
        method = GET;
    else if (methodString == "POST")
        method = POST;
    else if (methodString == "OPTIONS")
        method = OPTIONS;
    else if (methodString == "HEAD")
        method = HEAD;
    else if (methodString == "PUT")
        method = PUT;
    else if (methodString == "PATCH")
        method = PATCH;
    else if (methodString == "DELETE")
        method = DELETE;
    else if (methodString == "CONNECT")
        method = CONNECT;
    else
        method = UNKNOWN;
    return method;
}
