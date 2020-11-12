#pragma once

#include <memory>
#include <vector>
#include <queue>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "TaskBuilder.hpp"
#include "TasksController.hpp"
#include "Worker.hpp"

class Master {
 protected:
    Socket socket;

    std::vector<Worker> workers;

    TasksController controller;
    std::vector<Task>& haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;
    std::queue<std::unique_ptr<Task>>& haveData;
    std::shared_ptr<std::mutex> haveDataMutex;

    std::queue<HTTPClient> unprocessedClients;
    std::shared_ptr<std::mutex> unprocessedClientsMutex;
    TaskBuilder builder;

    virtual void Listen();

    std::thread masterThread;
    bool stop;

 public:
    Master();
    virtual ~Master();

    //  static void SetSocket(std::unique_ptr<Socket> socket);
    virtual void Start();
    virtual void Stop();
};
