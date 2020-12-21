#pragma once

#include <string>
#include <vector>
#include <queue>
#include <memory>
#include "socket.hpp"

class HTTPClient {
 private:
    std::string header;
    std::vector<char> body;

    std::shared_ptr<Socket> socket;

    std::vector<char>::iterator parseBuffer(std::vector<char>& buf, std::string& result);

 public:
    explicit HTTPClient(std::shared_ptr<Socket> socket);
    explicit HTTPClient(int port, int queueSize);

    std::string getHeader() const { return header; }
    std::vector<char> getBody() const { return body; }
    std::queue<std::string> getBodyQueue(const std::string& separator = "|") const;

    void setHeader(std::string header) { this->header = std::move(header); }
    void setBody(std::vector<char> body) { this->body = std::move(body); }
    void setBody(std::queue<std::string>& bodyQueue, const std::string& separator = "|");

    void recvHeader();
    void recvBody(size_t contentLength);

    void send(bool close = false);
    void send(std::vector<char> data, bool close = false);

    bool hasData() const { return socket->hasData(); }

    int getPort() const;
    int getSd() const;
};
