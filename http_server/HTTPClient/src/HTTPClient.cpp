#include "HTTPClient.hpp"
#include <iostream>

void HTTPClient::recvHeader(std::shared_ptr<Socket> client) {
    std::string result;
    while (result.find("\r\n\r\n") == std::string::npos &&
           result.find("\n\n") == std::string::npos) {
        result += client->recv();
    }

    size_t bodyStartIndex = result.find("\r\n\r\n");
    size_t shift = 4;
    if (bodyStartIndex == std::string::npos) {
        bodyStartIndex = result.find("\n\n");
        shift = 2;
    }

    header = result.substr(0, bodyStartIndex + shift / 2);
    body = result.substr(bodyStartIndex + shift, std::string::npos);
    return;
}
