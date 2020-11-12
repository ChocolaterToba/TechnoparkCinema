#pragma once;

#include <memory>
#include <thread>
#include <queue>
#include "Task.hpp"

class TaskBuilder {
 private:
    std::queue<HTTPClient>& unprocessedClients;
    std::shared_ptr<std::mutex> unprocessedClientsMutex;
    std::vector<Task>& haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;

    void CreateTasks();

    std::thread builderThread;
    bool stop;

 public:
    TaskBuilder(std::queue<HTTPClient>& unprocessedClients,
                std::shared_ptr<std::mutex> unprocessedClientsMutex,
                std::vector<Task>& haveNoData,
                std::shared_ptr<std::mutex> haveNoDataMutex);
    ~TaskBuilder();

    void Start();
    void Stop();
};
