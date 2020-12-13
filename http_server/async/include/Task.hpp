#pragma once

#include <functional>
#include <vector>
#include "HTTPClient.hpp"

typedef std::function<std::function<void(std::vector<char>&)>
                      (std::vector<char>&, HTTPClient&)> PreFuncType;
typedef std::function<void(std::vector<char>&)> MainFuncType;
typedef std::function<void(std::vector<char>&, HTTPClient&)> PostFuncType;

class Task {
 protected:
    PreFuncType preFunc;
    MainFuncType mainFunc;
    PostFuncType postFunc;

    HTTPClient& client;

 public:
    explicit Task(HTTPClient client);
    virtual ~Task();

    virtual PreFuncType GetPreFunc();
    virtual MainFuncType GetMainFunc();
    virtual PostFuncType GetPostFunc();

    virtual void SetPreFunc(PreFuncType preFunc);
    virtual void SetMainFunc(MainFuncType mainFunc);
    virtual void SetPostFunc(PostFuncType postFunc);

    virtual HTTPClient& GetClient();

    virtual bool HasData();
};
