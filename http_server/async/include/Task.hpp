#pragma once

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include "HTTPClient.hpp"

using std::vector;
using std::string;

typedef std::function<void(std::map<string, string>&, vector<char>&,
                           std::map<int, HTTPClient>&, std::shared_ptr<std::mutex>,
                           HTTPClient&, HTTPClient&)> MainFuncType;
typedef std::function<MainFuncType
                      (std::map<string, string>&,
                       vector<char>&,
                       HTTPClient&)> PreFuncType;
typedef std::function<void(std::map<string, string>&,
                           vector<char>&,
                           HTTPClient&)> PostFuncType;

class Task {
 protected:
    PreFuncType preFunc;
    MainFuncType mainFunc;
    PostFuncType postFunc;

    // If we use something other that HTTP
    // as a protocol (like for DB connection)
    // then HTTPClient needs to be replaced with Client
    // which will be client abstraction.
    std::shared_ptr<HTTPClient> input;
    std::shared_ptr<HTTPClient> output;

 public:
    Task();
    explicit Task(HTTPClient& input);
    virtual ~Task() = default;

    virtual PreFuncType GetPreFunc();
    virtual MainFuncType GetMainFunc();
    virtual PostFuncType GetPostFunc();

    virtual void SetPreFunc(PreFuncType _preFunc);
    virtual void SetMainFunc(MainFuncType _mainFunc);
    virtual void SetPostFunc(PostFuncType _postFunc);

    virtual HTTPClient& GetInput();
    virtual HTTPClient& GetOutput();
    virtual void SetInput(HTTPClient& _input);
    virtual void SetOutput(HTTPClient& _output);

    virtual bool HasData();
};
