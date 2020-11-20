#pragma once

#include <string>
#include <vector>
#include "StaticBuilder.hpp"

class AuthorizationPage: public StaticBuilder {
 protected:
    bool AuthSuccess;
    virtual void AddErrorCode();
    
 public:
    AuthorizationPage();

    virtual void ParseRequestData();
    virtual void ReceiveMetadata();
    virtual void CreateResponseData();
    virtual std::vector<char>& GetResponseData();
}