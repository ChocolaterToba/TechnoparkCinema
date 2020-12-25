#pragma once

#include <string>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <memory>
#include "socket.hpp"

using std::vector;
using std::string;

class HTTPClient {
 private:
    string header;
    vector<char> body;

    std::shared_ptr<Socket> socket;

    vector<char>::iterator parseBuffer(vector<char> &buf, string &result);

 public:
    explicit HTTPClient(std::shared_ptr<Socket> socket);  // "We" are server
    explicit HTTPClient(int port, int queueSize);  // "We" are server
    explicit HTTPClient(const string &host, int port);  // "We" are client
    HTTPClient();

    string getHeader() const { return header; }
    vector<char> getBody() const { return body; }
    std::queue<string> getBodyQueue(const string &separator = "|") const;

    static std::queue<string> splitVectorToQueue(const vector<char> &origin, const string &separator = "|");
    static vector<char> mergeQueueToVector(std::queue<string> &origin, const string &separator = "|");

    static std::set<string> splitVectorToSet(const vector<char> &origin, const string &separator = "|");
    static vector<char> mergeSetToVector(std::set<string> &origin, const string &separator = "|");

    static std::map<string, string> splitVectorToMap(const vector<char> &origin,
                                                               const string &separator = "|",
                                                               const string &pairSeparator = ": ");
    static vector<char> mergeMapToVector(std::map<string, string> &origin,
                                              const string &separator = "|",
                                              const string &pairSeparator = ": ");

    void setHeader(string _header) { this->header = std::move(_header); }
    void setBody(vector<char> _body) { this->body = std::move(_body); }
    void setBody(std::queue<string> &bodyQueue, const string &separator = "|");

    void recvHeader();
    void recvBody(size_t contentLength);

    void send(bool close = false);
    void send(vector<char> data, bool close = false);

    bool hasData() const { return socket->hasData(); }

    int getPort() const;
    int getSd() const;

    void clear();
};
