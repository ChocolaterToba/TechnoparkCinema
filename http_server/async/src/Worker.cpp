#include <thread>
#include <mutex>
#include <functional>
#include <memory>
#include <vector>
#include <queue>

#include "Worker.hpp"
#include "Task.hpp"

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
                if (tasksMutex.try_lock()) {
                    tasksMutex.lock();
                    task = tasks.front();
                    tasks.pop_front();
                    tasksMutex.unlock();
                    state = TaskReceived;
                } else {
                    // wait(); TODO
                }
            } else {
                // wait(); TODO
            }
        }
    } else {
        throw std::exception();  //  Will implement later.
    }
}

void Worker::RunPreFunc() {
    if (state == TaskRecieved) {
        state = PreFuncRunning;
        currentTask.SetMainFunc(currentTask.GetPreFunc()(data, client));
        state = PreFuncRan;
    } else {
        throw std::exception();  //  Will implement later.
    }
}
void Worker::RunMainFunc() {
    if (state == PreFuncRan) {
        state = MainFuncRunning;
        currentTask.GetMainFunc()(data);
        state = MainFuncRan;
    } else {
        throw std::exception();  //  Will implement later.
    }
}
void Worker::RunPostFunc() {
    if (state == MainFuncRan) {
        state = PostFuncRunning;
        currentTask.GetPostFunc()(data, client);
        state = NoTask;
    } else {
        throw std::exception();  //  Will implement later.
    }
}

void WorkerLoop() {
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
        workerThread = std::thread(TaskLoop);
    }
}
void Worker::Stop() {
    if (!stop) {
        stop = true;
        workerThread.join();
    }
}
