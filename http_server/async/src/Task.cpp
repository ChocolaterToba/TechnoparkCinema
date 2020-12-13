#include <functional>
#include <vector>
#include "HTTPClient.hpp"
#include "Task.hpp"
#include "TaskFuncs.hpp"

Task::Task(HTTPClient client) : preFunc(PreProcess),
                                mainFunc(MainProcessBasic),
                                postFunc(PostProcess),
                                client(client) {}

PreFuncType Task::GetPreFunc() {
    return preFunc;
}
MainFuncType Task::GetMainFunc() {
    return mainFunc;
}
PostFuncType Task::GetPostFunc() {
    return postFunc;
}

void Task::SetPreFunc(PreFuncType preFunc) {
    this->preFunc = preFunc;
}
void Task::SetMainFunc(MainFuncType mainFunc) {
    this->mainFunc = mainFunc;
}
void Task::SetPostFunc(PostFuncType postFunc) {
    this->postFunc = postFunc;
}

HTTPClient& Task::GetClient() {
    return client;
}
bool Task::HasData() {
    return client.hasData();
}

