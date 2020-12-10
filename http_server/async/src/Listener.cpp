#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "Listener.hpp"

Listener::Listener(int port,
                   std::queue<HTTPClient>& unprocessedClients,
                   std::shared_ptr<std::mutex> unprocessedClientsMutex) :
          socket(),
          unprocessedClients(unprocessedClients),
          unprocessedClientsMutex(unprocessedClientsMutex),
          stop(true) {
              socket.createServerSocket(port, 5);
          }

Listener::~Listener() {
    Stop();
}

void Listener::Stop() {
    if (!stop) {
        stop = true;
        listenerThread.join();
    }
}

void Listener::Start() {
    if (stop) {
        stop = false;
        listenerThread = std::thread(&Listener::Loop, this);
    }
}

void Listener::Loop() {
    while (!stop) {
        HTTPClient client(socket.accept());
        unprocessedClientsMutex->lock();
        unprocessedClients.push(std::move(client));
        unprocessedClientsMutex->unlock();
    }
}


