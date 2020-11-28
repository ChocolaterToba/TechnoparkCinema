#pragma once

#include <string>
#include <vector>
#include <map>
#include "StaticBuilder.hpp"

typedef std::map<std::string, std::string> UserMatadata;
typedef std::map<std::string, std::string> FilmData;

class RecommendationsPage: public StaticBuilder {
 protected:
    UserMetadata userMetadata;
    std::vector<FilmData> films;
    void RemoveWatchedMovies();
    //  Will add more as page's functionality grows.

 public:
    RecommendationsPage(FCGIClient dbClient, FCGIClient videoFilesClient);

    virtual void ParseRequestData(FastCGIData requestData);
    virtual void AddMetadata();
    virtual void CreateResponseData();
    virtual FastCGIData GetResponseData();
}
