#pragma once;

#include <memory>
#include <vector>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "TaskBuilder.hpp"

class Master {
 private:
    Socket socket;

    std::vector<std::shared_ptr<Worker>> workers;

    std::unique_ptr<TaskBuilder> builder;
    std::shared_ptr<Queue<HTTPClient>> unprocessedClients;

    std::unique_ptr<ConectionsController> controller;
    Queue<Task>& haveNoData;
    Queue<Task>& haveData;

    void Listen();

    std::thread mainThread;
    bool stop;

 public:
    explicit Master(std::unique_ptr<Socket> socket =
                           std::make_unique<Socket>());

    //  static void SetSocket(std::unique_ptr<Socket> socket);
    void Start();
    void Stop();
};
