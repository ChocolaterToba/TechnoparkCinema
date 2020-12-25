#pragma once

#include <unistd.h>    // close()
#include <memory>
#include <string>
#include <vector>
#include <functional>

#include <iostream>

using std::vector;
using std::string;

string int2ipv4(uint32_t ip);

class Socket {
 public:
    Socket() : m_Sd(-1) {}
    explicit Socket(int sd, int port = 0) : m_Sd(sd), port(port), queue_size(0) {}

    Socket(const Socket& other) = delete;
    Socket(Socket&& other);

    Socket& operator=(const Socket& other) = delete;
    Socket& operator=(Socket&& other);
    ~Socket() {
        if (m_Sd > 0)
           ::close(m_Sd);
    }

 public:
    int sd() const noexcept { return m_Sd; }

    void connect(const string& host, int _port);
    void connect(const string& host,
                 int _port,
                 int timeout);

    void send(const string& s);
    void send(const vector<char>& s);

    bool hasData();

    int getPort() { return port; }

    string recv();
    string recv(size_t bytes);
    string recvTimed(int timeout);
    string recvLoop();

    vector<char> recvVector();
    vector<char> recvVector(size_t bytes);
    vector<char> recvVectorTimed(int timeout);
    vector<char> recvVectorLoop();

    void setRcvTimeout(int sec, int microsec);
    void setNonBlocked(bool opt);
    void setReuseAddr(int sd);

    void createServerSocket(uint32_t _port,
                            uint32_t listen_queue_size);
    std::shared_ptr<Socket> accept();
    void close() {
       ::close(m_Sd);
       m_Sd = -1;}

    void httpQuery(const string& query,
                   std::function<void(const string& s)> cb);

//    string httpHeader();  TODO: not implemented yet

 private:
    int m_Sd;
    uint32_t port;
    uint32_t queue_size;
};
