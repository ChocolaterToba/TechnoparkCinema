#pragma once

class FCGIClient {
 private:
    int socket;  //  int will be replaced with proper type

 public:
    FCGIClient(int socket);

    void listen();
    void receivePackage();
    void sendPackage();
};
