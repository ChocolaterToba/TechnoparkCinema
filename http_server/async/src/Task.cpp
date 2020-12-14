#include <functional>
#include <vector>
#include "HTTPClient.hpp"
#include "Task.hpp"
#include "TaskFuncs.hpp"

Task::Task() :
      preFunc(PreProcess),
      mainFunc(MainProcessBasic),
      postFunc(PostProcess) {}

Task::Task(HTTPClient& input) : Task() {
    this->input = std::make_shared<HTTPClient>(input);
}

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

HTTPClient& Task::GetInput() {
    return *input;
}
HTTPClient& Task::GetOutput() {
    return *output;
}

void Task::SetInput(HTTPClient& input) {
    this->input = std::make_shared<HTTPClient>(input);
}
void Task::SetOutput(HTTPClient& output) {
    this->output = std::make_shared<HTTPClient>(output);
}

bool Task::HasData() {
    return input->hasData();
}

