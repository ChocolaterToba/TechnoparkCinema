#pragma once

#include <string>
#include <vector>
#include "StaticBuilder.hpp"

class AuthorizationPage: public StaticBuilder {
 protected:
    bool AuthSuccess;
    virtual void AddErrorCode();
    
 public:
    AuthorizationPage(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(std::map<std::string, std::string> requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual std::map<std::string, std::string> GetResponseData();
}