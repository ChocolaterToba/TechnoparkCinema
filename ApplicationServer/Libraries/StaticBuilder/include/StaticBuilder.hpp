#pragma once

#include <string>
#include <vector>
#include "FSGIClient.h"

class StaticBuilder {
 protected:
    std::string pageType;
    std::vector<char> requestData;
    std::vector<char> responseData;  

 public:
    explicit StaticBuilder();

    virtual void ParseRequestData(std::string pageType, std::vector<char> requestData);
    virtual void ParseMetadata(std::vector<char> metadata);
    virtual void CreateResponseData();
    virtual std::vector<char>& GetResponseData();
};
