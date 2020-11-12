#include <memory>
#include <thread>
#include <queue>
#include "Task.hpp"
#include "TaskBuilder.hpp"

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
            unprocessedClientsMutex.lock();
            Task newTask(unprocessedClients.front());
            unprocessedClients.pop_front();
            unprocessedClientsMutex.unlock();
            haveNoDataMutex.lock();
            haveNoData.push_back(newTask);
            haveNoDataMutex.unlock();
        } else {
            // wait(); TODO
        }
    }
}

void TaskBuilder::Start() {
    if (stop) {
        stop = false;
        builderThread = std::thread(CreateTasks);
    }
}
void TaskBuilder::Stop() {
    if (!stop) {
        stop = true;
        builderThread.join();
    }
}