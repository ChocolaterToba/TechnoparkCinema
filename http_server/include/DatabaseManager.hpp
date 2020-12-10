#pragma once

#include <map>
#include <string>


class DatabaseManager {
 public:
    explicit DatabaseManager(const std::ifstream &source);

    std::string GetHeader(std::basic_string<char> header_name) const;
    std::string GetURL() const;
    std::string GetHTTPVersion() const;


 private:
    std::string url;
    std::string http_version;
    std::map<std::string, std::string> headers;

    void CheckRequestMethod(const std::string &method_name);
};
