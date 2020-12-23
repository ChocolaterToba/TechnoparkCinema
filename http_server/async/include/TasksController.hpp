#pragma once

#include <queue>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>
#include "Task.hpp"

class TasksController {
 protected:
    std::vector<Task>& haveNoData;
    std::shared_ptr<std::mutex> haveNoDataMutex;
    std::queue<Task>& haveData;
    std::shared_ptr<std::mutex> haveDataMutex;

    std::thread tasksControllerThread;
    virtual void Loop();
    bool stop;

 public:
    TasksController(std::vector<Task>& haveNoData,
                    std::shared_ptr<std::mutex> haveNoDataMutex,
                    std::queue<Task>& haveData,
                    std::shared_ptr<std::mutex> haveDataMutex);
    virtual ~TasksController();

    virtual void Start();
    virtual void Stop();
};
