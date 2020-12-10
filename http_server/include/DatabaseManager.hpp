#pragma once

#include <map>
#include <string>
#include <queue>

// not started yet - сейчас это временный фейк.
class DatabaseManager {
 public:
    explicit DatabaseManager();

    std::queue<std::string> GetParameters(std::queue<std::string> names);

    //вообще не долнжны быть тут
    std::queue<std::string> GetTemplates(std::queue<std::string> names);


 private:

};
