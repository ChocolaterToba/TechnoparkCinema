#pragma once

#include <string>
#include <map>
#include "StaticBuilder.hpp"

typedef std::map<std::string, std::string> UserMatadata;
typedef std::map<std::string, std::string> FilmData;
typedef std::map<std::string, std::string> PlayerSettings;
typedef std::map<std::string, std::string> RoomSettings;

class RoomPage: public StaticBuilder {
 protected:
    UserMetadata userMetadata;
    FilmData film;
    PlayerSettings playerSettings;
    RoomSettings roomSettings;
    //  Will add more as page's functionality grows.

 public:
    RoomPage(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(FastCGIData requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual FastCGIData GetResponseData();
}
