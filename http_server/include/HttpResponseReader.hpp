#pragma once

#include <map>
#include <string>

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
    explicit HttpResponseReader(const std::string& message);
    ~HttpResponseReader() = default;

    std::string GetHeaderValue(const std::string& header_name) const;
    std::string GetReturnCode() const;
    std::map<std::string, std::string> GetAllHeaders() const;
    size_t GetContentLength() const;

    //    std::string GetURL() const;  // ???
private:
    std::map<std::string, std::string> headers;
    std::string return_code;

    std::string GetReturnCode(const std::string& message) const;
    std::map<std::string, std::string> ExtractHeaders(const std::string& message);
};


