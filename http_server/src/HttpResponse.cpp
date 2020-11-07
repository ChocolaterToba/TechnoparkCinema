#include "HttpResponse.hpp"
#include "HttpResponse.hpp"
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
                response_string = GetHTML(request.GetURL());
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
                html_file = GetHTML(request.GetURL());
                return_code = "200 OK";
                http_version = "1.1";
                headers.insert(
                        std::pair<std::string, std::string>(std::string("Content-type"),
                        std::string("text/html; charset=UTF-8")));
            }
            catch (FileNotFoundException&) {
                return_code = "404 Not Found";
                html_file = "";
                http_version = "";
            }
            break;
        default:
            return_code = "501 Not Implemented";
    }

    FormResponseString();
}

std::string HttpResponse::GetHTML(const std::string &url) const {
    return std::string("<!doctype html>\n"
                       "<html lang=\"en\">\n"
                       "    <head>\n"
                       "        <title>Askme-ermilov</title>\n"
                       "    </head>\n"
                       "    \n"
                       "<body><h1>Hello world</h1>\n"
                       "</body>\n"
                       "</html>");
}

void HttpResponse::FormResponseString() {
    response_string.append("HTTP/").append(http_version).append(" ");
    response_string.append(return_code).append(CRLF);
    for (auto &header : headers) {
        response_string.append(header.first).append(": ").append(header.second).append(CRLF);
    }
    if (!html_file.empty())
        response_string.append(CRLF).append(html_file).append(CRLF);
}
