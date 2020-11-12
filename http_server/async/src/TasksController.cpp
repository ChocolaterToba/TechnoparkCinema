#include <queue>
#include <mutex>
#include <thread>
#include <memory>
#include "Task.hpp"
#include "TasksController.hpp"

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

void TasksControllerLoop() {
    while (!stop) {
        // TODO(Aglicheev): listening to queue.
    }
}

void TasksController::Start() {
    if (stop) {
        stop = false;
        tasksControllerThread = std::thread(TasksControllerLoop);
    }
}
void TasksController::Stop() {
    if (!stop) {
        stop = true;
        tasksControllerThread.join();
    }
}