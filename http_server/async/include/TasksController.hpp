#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>
#include "Task.hpp"

class TasksController {
 private:
    std::vector<Task> haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;
    std::queue<Task> haveData;
    std::shared_ptr<std::mutex> haveDataMutex;

    std::thread tasksControllerThread;
    void TasksControllerLoop();
    bool stop;

 public:
    TasksController();
    ~TasksController();

    std::vector<Task>& GetHaveNoData();
    std::shared_ptr<std::mutex> GetHaveNoDataMutex();
    std::queue<Task>& GetHaveData();
    std::shared_ptr<std::mutex> GetHaveDataMutex();

    void Start();
    void Stop();
}