#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <vector>
#include <queue>

#include "Worker.hpp"

Worker::Worker(std::queue<Task>& tasks, std::mutex tasksMutex) :
        this->tasks(tasks),
        this->tasksMutex(tasksMutex),
        state(NoTask),
        stop(true) {}

Worker::~Worker() {
    switch (state) {
        case NoTask, PreFuncRan, MainFuncRan:
            break;
        default:
            Stop();
    }
}

void Worker::RunPreFunc() {
    if (state == TaskReceived) {
        currentTask.SetMainFunc(currentTask.GetPreFunc()(data, client));
    } else {
        throw std::exception();  //  Will implement later.
    }
}
void RunMainFunc() {
    if (state == PreFuncRan) {
        currentTask.GetMainFunc()(data);
    } else {
        throw std::exception();  //  Will implement later.
    }
}
void RunPostFunc() {
    if (state == MainFuncRan) {
        currentTask.GetPostFunc()(data, client);
    } else {
        throw std::exception();  //  Will implement later.
    }
}

void TakeNewTask() {
    //TODO
}

void Start() {
    if (stop) {
        stop = false;
        workerThread = std::thread(TakeNewTask);
    }
}
void Stop() {
    if (!stop) {
        stop = true;
        workerThread.join();
    }
}
