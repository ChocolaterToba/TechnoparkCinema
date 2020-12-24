#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>
#include <fstream>

#include "socket.hpp"
#include "HTTPClient.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include "Master.hpp"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " port" << std::endl;
        return 0;
    }

    try {
        std::map<std::string, int> ports;
        ports["external"] = 5555;
        ports["from_database"] = 6666;
        ports["to_database"] = 7777;
        Master master(ports, 4);
        master.Start();
        sleep(300);  // replace with graceful shutdown.
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}