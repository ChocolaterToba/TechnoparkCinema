#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>
#include "Task.hpp"

class TasksController {
 protected:
    std::vector<Task> haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;
    std::queue<std::unique_ptr<Task>> haveData;
    std::shared_ptr<std::mutex> haveDataMutex;

    std::thread tasksControllerThread;
    virtual void TasksControllerLoop();
    bool stop;

 public:
    TasksController();
    virtual ~TasksController();

    virtual std::vector<Task>& GetHaveNoData();
    virtual std::shared_ptr<std::mutex> GetHaveNoDataMutex();
    virtual std::queue<std::unique_ptr<Task>>& GetHaveData();
    virtual std::shared_ptr<std::mutex> GetHaveDataMutex();

    virtual void Start();
    virtual void Stop();
};
