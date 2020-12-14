#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>
#include <fstream>

#include "socket.hpp"
#include "HTTPClient.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

#include "Listener.hpp"

void clientWork(HTTPClient client, bool* shutdown) {
    try {
        std::cout << "Starting new recv" << std::endl;
        client.recvHeader();

        HttpRequest request(client.getHeader());
        HttpResponse response(request);

        client.send(std::move(response.GetData()));
        std::cout << "Ending recv" << std::endl << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " port" << std::endl;
        return 0;
    }
    int port = std::stoi(std::string(argv[1]));
    std::vector<std::thread> threads;

    try {
        std::queue<HTTPClient> unprocessedClients;
        std::shared_ptr<std::mutex> unprocessedClientsMutex = std::make_shared<std::mutex>();

        Listener listener(port, unprocessedClients, unprocessedClientsMutex);
        listener.Start();

        bool shutdown = false;

        while (!shutdown) {
            unprocessedClientsMutex->lock();
            if (unprocessedClients.empty()) {
                unprocessedClientsMutex->unlock();
                std::cerr << "No new connections" << std::endl;
                sleep(3);  // wait for 3 seconds
                continue;
            }

            std::cout << "start new accept \n";
            threads.push_back(std::thread(clientWork, std::move(unprocessedClients.front()), &shutdown));
            unprocessedClients.pop();
            unprocessedClientsMutex->unlock();
        }
        for (size_t i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}