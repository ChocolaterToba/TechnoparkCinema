#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <vector>
#include "HTTPClient.hpp"
#include "Queue.hpp"

class Worker {
 private:
    Queue<Task>& tasks;
    std::unique_ptr<Task> currentTask;

    std::vector<char> data;

    std::thread workerThread;
    bool stop;

 public:
    explicit Worker(Queue<Task>& tasks);
    ~Worker();

    void SetPreFunc(std::function<void(HTTPClient&)> PreFunc);
    void SetMainFunc(std::function<void()> MainFunc);
    void SetPostFunc(std::function<void(HTTPClient&)> PostFunc);

    void RunPreFunc();
    void RunMainFunc();
    void RunPostFunc();

    void TakeNewTask();

    void Start();
    void Stop();
};
