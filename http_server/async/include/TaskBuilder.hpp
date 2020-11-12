#pragma once

#include <memory>
#include <thread>
#include <queue>
#include "Task.hpp"

class TaskBuilder {
 protected:
    std::queue<HTTPClient>& unprocessedClients;
    std::shared_ptr<std::mutex> unprocessedClientsMutex;
    std::vector<Task>& haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;

    virtual void CreateTasks();

    std::thread builderThread;
    bool stop;

 public:
    TaskBuilder(std::queue<HTTPClient>& unprocessedClients,
                std::shared_ptr<std::mutex> unprocessedClientsMutex,
                std::vector<Task>& haveNoData,
                std::shared_ptr<std::mutex> haveNoDataMutex);
    virtual ~TaskBuilder();

    virtual void Start();
    virtual void Stop();
};
