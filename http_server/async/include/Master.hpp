#pragma once;

#include <memory>
#include <vector>
#include <queue>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "TaskBuilder.hpp"
#include "ConnectionsController.hpp"

class Master {
 private:
    Socket socket;

    std::vector<Worker> workers;

    TaskBuilder builder;
    std::queue<HTTPClient> unprocessedClients;
    std::mutex unprocessedClientsMutex;


    ConnectionsController controller;
    std::vector<Task>& haveNoData;
    std::mutex& haveNoDataMutex;
    std::queue<Task>& haveData;
    std::mutex& haveDataMutex;

    void Listen();

    std::thread masterThread;
    bool stop;

 public:
    explicit Master(std::unique_ptr<Socket> socket);

    //  static void SetSocket(std::unique_ptr<Socket> socket);
    void Start();
    void Stop();
};
