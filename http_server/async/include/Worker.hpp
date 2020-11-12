#pragma once

#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <vector>
#include <queue>
#include "HTTPClient.hpp"
#include "Task.hpp"

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
    std::queue<std::unique_ptr<Task>>& tasks;
    std::shared_ptr<std::mutex> tasksMutex;
    std::unique_ptr<Task> currentTask;
    WorkerStates state;

    std::vector<char> data;

    std::thread workerThread;
    bool stop;

 public:
    Worker(std::queue<std::unique_ptr<Task>>& tasks,
           std::shared_ptr<std::mutex> tasksMutex);
    virtual ~Worker();

    virtual void WorkerLoop();

    virtual void TakeNewTask();

    virtual void RunPreFunc();
    virtual void RunMainFunc();
    virtual void RunPostFunc();

    virtual void Start();
    virtual void Stop();
};
