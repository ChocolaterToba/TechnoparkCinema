#pragma once

#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <memory>
#include "socket.hpp"

class HTTPClient {
 private:
    std::string header;
    std::vector<char> body;

    std::shared_ptr<Socket> socket;

    std::vector<char>::iterator parseBuffer(std::vector<char>& buf, std::string& result);

 public:
    explicit HTTPClient(std::shared_ptr<Socket> socket);  // "We" are server
    explicit HTTPClient(int port, int queueSize);  // "We" are server
    explicit HTTPClient(const std::string& host, int port);  // "We" are client
    HTTPClient();

    std::string getHeader() const { return header; }
    std::vector<char> getBody() const { return body; }
    std::queue<std::string> getBodyQueue(const std::string& separator = "|") const;

    static std::queue<std::string> splitVectorToQueue(const std::vector<char>& origin, const std::string& separator = "|");
    static std::vector<char> mergeQueueToVector(std::queue<std::string>& origin, const std::string& separator = "|");

    static std::set<std::string> splitVectorToSet(const std::vector<char>& origin, const std::string& separator = "|");
    static std::vector<char> mergeSetToVector(std::set<std::string>& origin, const std::string& separator = "|");

    static std::map<std::string, std::string> splitVectorToMap(const std::vector<char>& origin,
                                                               const std::string& separator = "|",
                                                               const std::string& pairSeparator = ": ");
    static std::vector<char> mergeMapToVector(std::map<std::string, std::string>& origin,
                                              const std::string& separator = "|",
                                              const std::string& pairSeparator = ": ");

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

    void clear();
};
