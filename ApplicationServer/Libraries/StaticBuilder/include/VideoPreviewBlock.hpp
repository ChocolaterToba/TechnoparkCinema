#pragma once

#include <string>
#include <vector>
#include <map>
#include "StaticBuilder.hpp"

typedef std::map<std::string, std::string> UserMatadata;
typedef std::map<std::string, std::string> FilmData;

class VideoPreviewBlock: public StaticBuilder {
 protected:
    UserMetadata userMetadata;
    FilmData film;
    //  Will add more as page's functionality grows.

 public:
    VideoPreviewBlock(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(FastCGIData requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual FastCGIData GetResponseData();
}
