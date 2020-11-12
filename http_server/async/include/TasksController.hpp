#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include "Task.hpp"

class TasksController {
 private:
    std::vector<Task> haveNoData;
    std::mutex haveNoDataMutex;
    std::queue<Task> haveData;
    std::mutex haveDataMutex;

    std::thread tasksControllerThread;
    void TasksControllerLoop();
    bool stop;

 public:
    TasksController();
    ~TasksController();

    std::vector<Task>& GetHaveNoData();
    std::mutex& GetHaveNoDataMutex();
    std::queue<Task>& GetHaveData();
    std::mutex& GetHaveDataMutex();

    void Start();
    void Stop();
}