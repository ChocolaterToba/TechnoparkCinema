#pragma once

#include <functional>
#include "HTTPClient"

class Task {
 private:
    std::function<void(HTTPClient&)> preFunc;
    std::function<void()> mainFunc;
    std::function<void(HTTPClient&)> postFunc;

    HTTPClient& client;

 public:
    explicit Task(HTTPClient& client);

    std::function<void(HTTPClient&)> GetPreFunc();
    std::function<void()> GetMainFunc();
    std::function<void(HTTPClient&)> GetPostFunc();

    HTTPClient& GetClient();
}