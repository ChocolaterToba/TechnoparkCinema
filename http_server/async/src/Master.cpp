#include <memory>
#include <vector>
#include <queue>
#include <map>
#include "socket.hpp"
#include "HTTPClient.hpp"
#include "Task.hpp"
#include "TaskBuilder.hpp"
#include "TasksController.hpp"
#include "Listener.hpp"
#include "Master.hpp"

Master::Master(std::map<std::string, int>& ports, size_t workersAmount):
        workers(),
        ports(ports),
        listeners(),
        controller(),
        haveNoData(controller.GetHaveNoData()),
        haveNoDataMutex(controller.GetHaveNoDataMutex()),
        haveData(controller.GetHaveData()),
        haveDataMutex(controller.GetHaveDataMutex()),
        unprocessedClients(),
        unprocessedClientsMutex(std::make_shared<std::mutex>()),
        builder(unprocessedClients, unprocessedClientsMutex,
                haveNoData, haveNoDataMutex),
        stop(true) {
            // if (!workersAmount) {
            //     throw std::runtime_error(std::string(
            //         "Master constructor: cannot construct master with no workers"
            //     ));
            // }
            // for (size_t i = 0; i < workersAmount; ++i) {
            //     workers.emplace_back(haveData, haveDataMutex);
            // }
            for (auto& keyVal : ports) {
                listeners.emplace_back(keyVal.second, unprocessedClients, unprocessedClientsMutex);
            }
        }

Master::~Master() {
    Stop();
}

void Master::Start() {
    if (stop) {
        stop = false;
        // run listeners, workers, etc.
    }
}
void Master::Stop() {
    if (!stop) {
        stop = true;
        masterThread.join();
    }
}
