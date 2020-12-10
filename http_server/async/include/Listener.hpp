#pragma once

#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "socket.hpp"
#include "HTTPClient.hpp"

class Listener {
 private:
    Socket socket;
    std::queue<HTTPClient>& unprocessedClients;
    std::shared_ptr<std::mutex> unprocessedClientsMutex;

    bool stop;

    std::thread listenerThread;
 
 public:
    explicit Listener(int port,
                      std::queue<HTTPClient>& unprocessedClients,
                      std::shared_ptr<std::mutex> unprocessedClientsMutex);
    ~Listener();
    
    void Start();
    void Stop();
    void Loop();
};