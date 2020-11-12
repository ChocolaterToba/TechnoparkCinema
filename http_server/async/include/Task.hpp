#pragma once

#include <functional>
#include <vector>
#include "HTTPClient.hpp"
#include "Worker.hpp"

typedef std::function<std::function<void(std::vector<char>&)>
                      (std::vector<char>&, HTTPClient&)> PreFuncType;
typedef std::function<void(std::vector<char>&)> MainFuncType;
typedef std::function<void(std::vector<char>&, HTTPClient&)> PostFuncType;

class Task {
 private:
    PreFuncType preFunc;
    MainFuncType mainFunc;
    PostFuncType postFunc;

    HTTPClient& client;

 public:
    explicit Task(HTTPClient client);

    PreFuncType GetPreFunc();
    MainFuncType GetMainFunc();
    MainFuncType GetPostFunc();

    void SetPreFunc(PreFuncType preFunc);
    void SetMainFunc(MainFuncType mainFunc);
    void SetPostFunc(PostFuncType postFunc);

    HTTPClient& GetClient();
}
