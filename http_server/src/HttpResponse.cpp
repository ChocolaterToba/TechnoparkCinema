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
    response_string = std::string();
    if (request.GetHTTPVersion().empty()) {
        http_version = "0.9";
        if (request.GetRequestMethod() == GET) {
            try {
                response_string = GetHTML(request.GetURL());
                return;
            }
            catch (FileNotFoundException) {
                throw OldVersionException();
            }
        }
    } else {
        throw OldVersionException();
    }

    std::string html_file;
    switch (request.GetRequestMethod()) {
        case GET:
            try {
                html_file = GetHTML(request.GetURL());
            }
            catch (FileNotFoundException) {
                return_code = "404 Not Found";
            }
        default:
            return_code = "501 Not Implemented";
    }
}

std::string HttpResponse::GetHTML(std::string url) const {
    return std::string();
}
