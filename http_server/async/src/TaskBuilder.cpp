#include <memory>
#include <thread>
#include <queue>
#include <mutex>

#include "Task.hpp"
#include "TaskBuilder.hpp"
#include "msleep.hpp"

TaskBuilder::TaskBuilder(std::queue<HTTPClient>& unprocessedClients,
                         std::shared_ptr<std::mutex> unprocessedClientsMutex,
                         std::vector<Task>& haveNoData,
                         std::shared_ptr<std::mutex> haveNoDataMutex) :
    unprocessedClients(unprocessedClients),
    unprocessedClientsMutex(unprocessedClientsMutex),
    haveNoData(haveNoData),
    haveNoDataMutex(haveNoDataMutex),
    stop(true) {}

TaskBuilder::~TaskBuilder() {
    Stop();
}

void TaskBuilder::CreateTasks() {
    while (!stop) {
        if (!unprocessedClients.empty())  {
            //  TaskBuilder is singular, so there's no "queue shrinking" problem.
            unprocessedClientsMutex->lock();
            Task newTask(unprocessedClients.front());
            unprocessedClients.pop();
            unprocessedClientsMutex->unlock();
            haveNoDataMutex->lock();
            haveNoData.push_back(std::move(newTask));
            haveNoDataMutex->unlock();
        } else {
            msleep(30);
        }
    }
}

void TaskBuilder::Start() {
    if (stop) {
        stop = false;
        builderThread = std::thread(&TaskBuilder::CreateTasks, this);
    }
}
void TaskBuilder::Stop() {
    if (!stop) {
        stop = true;
        builderThread.join();
    }
}
