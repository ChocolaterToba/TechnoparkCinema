#pragma once

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>

class FCGIClient {
 private:
    int socket;  //  int will be replaced with proper type

 public:
    FCGIClient(int socket);

    void listen();
    std::vector<std::string> receivePackage();
    void sendPackage(std::vector<std::string>);
};
