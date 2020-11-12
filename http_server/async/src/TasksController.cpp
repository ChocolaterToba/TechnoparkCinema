#include <queue>
#include <mutex>
#include <thread>
#include "Task.hpp"
#include "TasksController.hpp"

TasksController::TasksController() :
    haveNoData(),
    haveNoDataMutex(),
    haveData(),
    haveDataMutex(),
    stop(true) {}

TasksController::~TasksController() {
    Stop();
    //  TODO(Aglicheev): add mutex handling.
}

std::vector<Task>& TasksController::GetHaveNoData() {
    return haveNoData;
}
std::mutex& TasksController::GetHaveNoDataMutex() {
    return haveNoDataMutex;
}
std::queue<Task>& TasksController::GetHaveData() {
    return haveData;
}
std::mutex& TasksController::GetHaveDataMutex() {
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
