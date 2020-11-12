#pragma once;

#include <memory>
#include <thread>
#include <queue>
#include "Task.hpp"

class TaskBuilder {
 private:
    std::queue<HTTPClient>& unprocessedClients;
    std::mutex& enprocessedClientsMutex
    std::queue<Task>& haveNoData;
    std::mutex& haveNoDataMutex;

    void CreateTasks();

    std::thread builderThread;
    bool stop;

 public:
    explicit TaskBuilder(std::queue<HTTPClient>& unprocessedClients,
                         std::mutex& enprocessedClientsMutex,
                         std::queue<Task>& haveNoData,
                         std::mutex& haveNoDataMutex,
                         );
    ~TaskBuilder();

    void Start();
    void Stop();
};
