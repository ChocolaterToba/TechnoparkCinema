#pragma once

#include <unistd.h>    // close()
#include <memory>
#include <string>
#include <vector>
#include <functional>

std::string int2ipv4(uint32_t ip);

class Socket {
 public:
    Socket() : m_Sd(-1) {}
    explicit Socket(int sd) : m_Sd(sd) {}
    ~Socket() {
        if (m_Sd > 0) {
           ::close(m_Sd);
        }
    }

 public:
    int sd() const noexcept { return m_Sd; }

    void connect(const std::string& host, int port);
    void connect(const std::string& host,
                 int port,
                 int timeout);

    void send(const std::string& s);
    void send(const std::vector<char>& s);

    bool hasData();

    std::string recv();
    std::string recv(size_t bytes);
    std::string recvTimed(int timeout);
    std::string recvLoop();

    std::vector<char> recvVector();
    std::vector<char> recvVector(size_t bytes);
    std::vector<char> recvVectorTimed(int timeout);
    std::vector<char> recvVectorLoop();

    void setRcvTimeout(int sec, int microsec);
    void setNonBlocked(bool opt);
    void setReuseAddr(int sd);

    void createServerSocket(uint32_t port,
                            uint32_t queue_size);
    std::shared_ptr<Socket> accept();
    void close() { ::close(m_Sd); }

    void httpQuery(const std::string& query,
                   std::function<void(const std::string& s)> cb);

    std::string httpHeader();

 private:
    // Socket(const Socket &s);
    // const Socket& operator=(const Socket &s);
 private:
    int m_Sd;
};