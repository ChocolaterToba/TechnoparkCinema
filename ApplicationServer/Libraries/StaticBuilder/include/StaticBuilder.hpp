#pragma once

#include <string>
#include <vector>
#include "FSGIClient.h"

class StaticBuilder {
 protected:
    std::string pageType;
    std::vector<char> requestData;
    std::vector<char> responseData;

    FCGIClient dbClient;
    FCGIClient videoFilesClient;

 public:
    StaticBuilder(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(std::map<std::string, std::string> requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual std::map<std::string, std::string> GetResponseData();
};
