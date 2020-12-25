#pragma once

#include <map>
#include <string>

#include "request_methods.hpp"

using std::string;
using std::map;

// string default value ""

/* 
0 2 4      13 15   20 22     
GET /index.php HTTP/1.1
Host: example.com
User-Agent: Mozilla/5.0 (X11; U; Linux i686; ru; rv:1.9b5) Gecko/2008050509 Firefox/3.0b5
Accept: text/html
Connection: close
*/

class HttpRequest {
 public:
    explicit HttpRequest(const string &message);

    string GetHeader(const std::basic_string<char> &header_name) const;
    ~HttpRequest() = default;
    string GetURL() const;
    string GetHTTPVersion() const;
    RequestMethod GetRequestMethod() const;
    string GetRequestMethodString() const;
    map<string, string> GetAllHeaders() const;
    int GetContentLength();

    static string RequestMethodToString(RequestMethod method);
    static RequestMethod StringToRequestMethod(const string& methodString);

 private:
    RequestMethod request_method;
    string url;
    string http_version;
    map<string, string> headers;

    void SetRequestMethod(const string &method_name);
};
