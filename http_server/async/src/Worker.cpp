#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <vector>
#include <queue>

#include "Worker.hpp"
#include "Task.hpp"
#include "msleep.hpp"

Worker::Worker(std::queue<Task>& tasks,
               std::shared_ptr<std::mutex> tasksMutex) :
        tasks(tasks),
        tasksMutex(tasksMutex),
        state(NoTask),
        stop(true) {}

Worker::~Worker() {
    Stop();
}

void Worker::TakeNewTask() {
    if (state == NoTask) {
        while (!stop) {
            if (!tasks.empty()) {
                if (tasksMutex->try_lock()) {
                    currentTask = std::move(tasks.front());
                    tasks.pop();
                    tasksMutex->unlock();
                    state = TaskRecieved;
                } else {
                    msleep(30);
                }
            } else {
                msleep(30);
            }
        }
    } else {
        throw std::runtime_error(std::string(
            "Worker: TakeNewTask: state is not NoTask!"));
    }
}

void Worker::RunPreFunc() {
    if (state == TaskRecieved) {
        state = PreFuncRunning;
        currentTask.SetMainFunc(currentTask.GetPreFunc()(data,
                                currentTask.GetInput()));
        state = PreFuncRan;
    } else {
        throw std::runtime_error(std::string(
            "Worker: RunPreFunc: state is not TaskRecieved!"));
    }
}
void Worker::RunMainFunc() {
    if (state == PreFuncRan) {
        state = MainFuncRunning;
        currentTask.GetMainFunc()(data, currentTask.GetOutput());
        state = MainFuncRan;
    } else {
        throw std::runtime_error(std::string(
            "Worker: RunMainFunc: state is not PreFuncRan!"));
    }
}
void Worker::RunPostFunc() {
    if (state == MainFuncRan) {
        state = PostFuncRunning;
        currentTask.GetPostFunc()(data, currentTask.GetOutput());
        state = NoTask;
    } else {
        throw std::runtime_error(std::string(
            "Worker: RunPostFunc: state is not MainFuncRan!"));
    }
}

void Worker::WorkerLoop() {
    while (!stop) {
        TakeNewTask();
        RunPreFunc();
        RunMainFunc();
        RunPostFunc();
    }
}

void Worker::Start() {
    if (stop) {
        stop = false;
        state = NoTask;
        workerThread = std::thread(&Worker::WorkerLoop, this);
    }
}
void Worker::Stop() {
    if (!stop) {
        stop = true;
        workerThread.join();
    }
}
