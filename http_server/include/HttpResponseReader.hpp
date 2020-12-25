#pragma once

#include <map>
#include <string>
#include <vector>

using std::string;
using std::map;
using std::vector;

//    HTTP/1.1 200 OK
//            Date: Wed, 11 Feb 2009 11:20:59 GMT
//            Server: Apache
//            X-Powered-By: PHP/5.2.4-2ubuntu5wm1
//    Last-Modified: Wed, 11 Feb 2009 11:20:59 GMT
//            Content-Language: ru
//            Content-Type: text/html; charset=utf-8
//    Content-Length: 1234
//    Connection: close
//    (пустая строка)
//    (запрошенная страница в HTML)

class HttpResponseReader {
public:
    explicit HttpResponseReader(const string &message);
    ~HttpResponseReader() = default;

    string GetHeaderValue(const string &header_name) const;
    string GetReturnCode() const;
    map<string, string> GetAllHeaders() const;
    size_t GetContentLength() const;
    vector<char> GetBody() const;

    //    string GetURL() const;  // ???
private:
    map<string, string> headers;
    string return_code;
    vector<char> body;

    string GetReturnCode(const string &message) const;
    map<string, string> ExtractHeaders(const string &message);
    vector<char> ExtractBody(const string &message) const;
};


