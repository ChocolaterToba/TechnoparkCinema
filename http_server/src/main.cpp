#include <iostream>
#include <stdexcept>
#include <vector>
#include <thread>

#include "socket.hpp"
#include "HTTPClient.hpp"
#include "HttpResponse.hpp"
#include "HttpRequest.hpp"

void clientWork(std::shared_ptr<Socket> client, bool* shutdown) {
    client->setRcvTimeout(/*sec*/ 120, /*microsec*/ 0);
    try {
        std::cout << "Starting new recv \n";
        HTTPClient httpclient;
        httpclient.recvHeader(client);
        HttpRequest request(httpclient.getHeader());
        std::cout << "-------------\n" << HttpResponse(request).GetString() << "-------------" << std::endl;
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
        Socket s;
        s.createServerSocket(port, 5);

        bool shutdown = false;
        size_t needToClean = 100;

        while (!shutdown) {
            if (threads.size() > needToClean) {
                needToClean *= 2;
                for (size_t i = threads.size() - 1; i > 0; --i) {
                    if (threads[i].joinable()) {
                        threads[i].join();
                        threads.erase(threads.begin() + i);
                    }
                }
                if (threads[0].joinable()) {
                    threads[0].join();
                    threads.erase(threads.begin() + 0);
                }
            }
            std::cout << "start new accept \n";
            threads.push_back(std::thread(clientWork, s.accept(), &shutdown));
        }
        for (size_t i = 0; i < threads.size(); ++i) {
            threads[i].join();
        }
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}