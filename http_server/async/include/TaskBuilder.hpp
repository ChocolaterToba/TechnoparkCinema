#pragma once;

#include <memory>
#include <thread>
#include "Queue.hpp"
#include "Task.hpp"

class TaskBuilder {
 private:
    std::shared_ptr<Queue<Task>> haveNoData;

    void CreateTasks();

    std::thread builderThread;
    bool stop;

 public:
    explicit TaskBuilder(std::shared_ptr<Queue<Task>> haveNoData);
    ~TaskBuilder();

    void Start();
    void Stop();
};
