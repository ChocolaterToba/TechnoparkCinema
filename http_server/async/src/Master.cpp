#include <memory>
#include <vector>
#include <queue>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "Task.hpp"
#include "TaskBuilder.hpp"
#include "TasksController.hpp"
#include "Master.hpp"

Master::Master():
        stop(true),
        unprocessedClientsMutex(std::make_shared<std::mutex>()),
        controller(),
        haveNoData(controller.GetHaveNoData()),
        haveNoDataMutex(controller.GetHaveNoDataMutex()),
        haveData(controller.GetHaveData()),
        haveDataMutex(controller.GetHaveDataMutex()),
        builder(haveNoData,
                haveNoDataMutex,
                haveData,
                haveDataMutex) {}

Master::~Master() {
    Stop();
}

void Listen() {
    // TODO(Aglicheev): connections listening.
}

void Start() {
    if (stop) {
        stop = false;
        masterThread = std::thread(Listen);
    }
}
void Stop() {
    if (!stop) {
        stop = true;
        masterThread.join();
    }
}
