#pragma once

#include <map>
#include <string>
#include <queue>
#include <set>
// not started yet - сейчас это временный фейк.
class DatabaseManager {
 public:
    explicit DatabaseManager();

    std::set<std::string> GetParameters(std::set<std::string> names);

    //вообще не долнжны быть тут
    std::queue<std::string> GetTemplates(std::queue<std::string> names);


 private:

};
