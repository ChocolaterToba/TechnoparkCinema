#include <string>
#include <map>

#include "exceptions.hpp"
#include "DatabaseManager.hpp"

DatabaseManager::DatabaseManager(const std::ifstream &source) {

}

std::string DatabaseManager::GetHeader(std::basic_string<char> string) const {
    auto iter = headers.find(string);
    if (iter == headers.end()) {
        return std::string();
    } else {
        return iter->second;
    }
}

void DatabaseManager::CheckRequestMethod(const std::string &method_name) {
    // if (method_name == "GET")
    //     request_method = GET;
    // else if (method_name == "POST")
    //     request_method = POST;
    // else if (method_name == "OPTIONS")
    //     request_method = OPTIONS;
    // else if (method_name == "HEAD")
    //     request_method = HEAD;
    // else if (method_name == "PUT")
    //     request_method = PUT;
    // else if (method_name == "PATCH")
    //     request_method = PATCH;
    // else if (method_name == "DELETE")
    //     request_method = DELETE;
    // else if (method_name == "CONNECT")
    //     request_method = CONNECT;
    // else
    //     request_method = UNKNOWN;
}

std::string DatabaseManager::GetURL() const {
    return url;
}

std::string DatabaseManager::GetHTTPVersion() const {
    return http_version;
}
