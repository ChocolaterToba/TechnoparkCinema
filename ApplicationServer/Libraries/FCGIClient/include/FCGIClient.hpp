#pragma once

#include <fastcgi2/component.h>
#include <fastcgi2/component_factory.h>
#include <fastcgi2/handler.h>
#include <fastcgi2/request.h>
#include <vector>
#include <string>

class FCGIClient {
 private:
    int socket;  //  int will be replaced with proper type

 public:
    FCGIClient(int socket);

    virtual void Listen();
    virtual std::map<std::string, std::string> ReceivePackage();
    virtual void SendPackage(std::map<std::string, std::string>);
};
