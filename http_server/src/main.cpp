#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>
#include <fstream>

#include "ports.hpp"

#include "socket.hpp"
#include "HTTPClient.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include "Master.hpp"

int main(int argc, char* argv[]) {
    try {
        std::map<std::string, int> ports;
        ports["external"] = EXTERNAL_PORT;
        ports["from_database"] = FROM_DB_PORT;
        ports["to_database"] = TO_DB_PORT;
        Master master(ports, 4);
        master.Start();
        sleep(300);  // replace with graceful shutdown.
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}
