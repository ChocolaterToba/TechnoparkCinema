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
        controller(),
        haveNoData(controller.GetHaveNoData()),
        haveNoDataMutex(controller.GetHaveNoDataMutex()),
        haveData(controller.GetHaveData()),
        haveDataMutex(controller.GetHaveDataMutex()),
        unprocessedClients(),
        unprocessedClientsMutex(std::make_shared<std::mutex>()),
        builder(unprocessedClients,
                unprocessedClientsMutex,
                haveNoData,
                haveNoDataMutex),
        stop(true) {}

Master::~Master() {
    Stop();
}

void Master::Listen() {
    // TODO(Aglicheev): connections listening.
}

void Master::Start() {
    if (stop) {
        stop = false;
        masterThread = std::thread(&Master::Listen, this);
    }
}
void Master::Stop() {
    if (!stop) {
        stop = true;
        masterThread.join();
    }
}
