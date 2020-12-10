#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <map>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "TaskBuilder.hpp"
#include "TasksController.hpp"
#include "Listener.hpp"
#include "Worker.hpp"

class Master {
 protected:
    Socket socket;

    std::vector<Worker> workers;

    std::map<std::string, int> ports;
    std::vector<Listener> listeners;

    TasksController controller;
    std::vector<Task>& haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;
    std::queue<std::unique_ptr<Task>>& haveData;
    std::shared_ptr<std::mutex> haveDataMutex;

    std::queue<HTTPClient> unprocessedClients;
    std::shared_ptr<std::mutex> unprocessedClientsMutex;
    TaskBuilder builder;

    std::thread masterThread;
    bool stop;

 public:
    explicit Master(std::map<std::string, int>& ports, size_t workersAmount = 1);
    virtual ~Master();

    //  static void SetSocket(std::unique_ptr<Socket> socket);
    virtual void Start();
    virtual void Stop();
};
