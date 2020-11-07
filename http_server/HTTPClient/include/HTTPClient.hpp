#pragma once

#include <string>
#include <memory>
#include "socket.hpp"

class HTTPClient {
 private:
    std::string header;
    std::string body;

 public:
    std::string getHeader() { return header; }
    std::string getBody() { return body; }
    void setHeader(std::string header) { this->header = header; }
    void setBody(std::string body) { this->body = body; }
    void recvHeader(std::shared_ptr<Socket> client);
};
