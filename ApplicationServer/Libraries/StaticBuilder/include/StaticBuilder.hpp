#pragma once

#include <string>
#include <vector>
#include <map>
#include "FSGIClient.h"

typedef std::map<std::string, std::string> FastCGIData;

class StaticBuilder {
 protected:
    std::string pageType;
    FastCGIData requestData;
    FastCGIData responseData;

    FCGIClient dbClient;
    FCGIClient videoFilesClient;

 public:
    StaticBuilder(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(FastCGIData requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual FastCGIData GetResponseData();
};
