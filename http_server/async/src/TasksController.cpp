#include <queue>
#include <mutex>
#include <thread>
#include <memory>

#include "Task.hpp"
#include "TasksController.hpp"
#include "msleep.hpp"

TasksController::TasksController() :
    haveNoDataMutex(std::make_shared<std::mutex>()),
    haveDataMutex(std::make_shared<std::mutex>()),
    stop(true) {}

TasksController::~TasksController() {
    Stop();
}

std::vector<Task>& TasksController::GetHaveNoData() {
    return haveNoData;
}
std::shared_ptr<std::mutex> TasksController::GetHaveNoDataMutex() {
    return haveNoDataMutex;
}
std::queue<Task>& TasksController::GetHaveData() {
    return haveData;
}
std::shared_ptr<std::mutex> TasksController::GetHaveDataMutex() {
    return haveDataMutex;
}

void TasksController::TasksControllerLoop() {
    while (!stop) {
        if (!haveNoData.empty()) {
            std::queue<Task> buffer;
            haveNoDataMutex->lock();
            int i = haveNoData.size();
            while (i >= 0) {
                if (haveNoData[i].HasData()) {
                    buffer.push(std::move(haveNoData[i]));
                    haveNoData.erase(haveNoData.begin() + i);
                }
                --i;
            }
            haveNoDataMutex->unlock();

            haveDataMutex->lock();
            while (!buffer.empty()) {
                haveData.push(std::move(buffer.front()));
                buffer.pop();
            }
            haveDataMutex->unlock();
        }

        msleep(120);
    }
}

void TasksController::Start() {
    if (stop) {
        stop = false;
        tasksControllerThread =
            std::thread(&TasksController::TasksControllerLoop, this);
    }
}
void TasksController::Stop() {
    if (!stop) {
        stop = true;
        tasksControllerThread.join();
    }
}
