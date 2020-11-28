#pragma once

#include <string>
#include <vector>
#include <map>
#include "StaticBuilder.hpp"

typedef std::map<std::string, std::string> UserMatadata;
typedef std::map<std::string, std::string> FilmData;

class HotVideosPage: public StaticBuilder {
 protected:
    UserMatadata userMetadata;
    std::vector<FilmData> films;
    void SortByHot();
    //  Will add more as page's functionality grows.

 public:
    HotVideosPage(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(FastCGIData requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual FastCGIData GetResponseData();
}
