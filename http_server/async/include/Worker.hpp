#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <vector>
#include <queue>
#include "HTTPClient.hpp"

typedef enum {
    NoTask,
    TaskRecieved,
    PreFuncRunning,
    PreFuncRan,
    MainFuncRunning,
    MainFuncRan,
    PostFuncRunning
} WorkerStates;

class Worker {
 private:
    std::queue<Task>& tasks;
    std::mutex tasksMutex;
    Task currentTask;
    WorkerStates state;

    std::vector<char> data;

    std::thread workerThread;
    bool stop;

 public:
    Worker(std::queue<Task>& tasks, std::mutex tasksMutex);
    ~Worker();

    void RunPreFunc();
    void RunMainFunc();
    void RunPostFunc();

    void TakeNewTask();

    void Start();
    void Stop();
};
