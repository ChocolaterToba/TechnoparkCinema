#include <memory>
#include <vector>
#include <queue>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "TaskBuilder.hpp"
#include "ConnectionsController.hpp"
#include "Master.hpp"

Master::Master(std::unique_ptr<Socket> socket):
        this->socket(socket),
        workers(),
        unprocessedClients(),
        unprocessedClientsMutex(),
        controller(),
        stop(true),
        masterThread() {
    haveNoData = controller.GetHaveNoData();
    haveNoDataMutex = controller.GetHaveNoDataMutex();
    haveData = controller.GetHaveData();
    haveDataMutex = controller.GetHaveDataMutex();
    builder(unporcessedClients, unprocessedClientsMutex,
            haveNoData, haveNoDataMutex);
}

Master::~Master() {
    Stop();
    // TODO(Aglicheev): mutex handling.
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
