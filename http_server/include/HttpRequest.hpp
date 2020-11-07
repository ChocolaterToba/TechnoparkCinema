#pragma once

#include <map>
#include <string>

typedef enum {
    UNKNOWN = -1,
    GET,
    POST,
    OPTIONS,
    HEAD,
    PUT,
    PATCH,
    DELETE,
    CONNECT,
} RequestMethod;

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
    HttpRequest(const std::string &message);

    std::string GetHeader(std::string &header_name) const;
    std::string GetURL() const;
    std::string GetHTTPVersion() const;
    RequestMethod GetRequestMethod() const;

 private:
    RequestMethod request_method;
    std::string url;
    std::string http_version;
    std::map<std::string, std::string> headers;

    void CheckRequestMethod(const std::string &method_name);
};

