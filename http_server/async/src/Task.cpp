#include <functional>
#include <vector>

#include "HTTPClient.hpp"
#include "Task.hpp"
#include "TaskFuncs.hpp"

Task::Task() :
      preFunc(PreProcess),
      mainFunc(MainProcessBasic),
      postFunc(PostProcess) {
    this->input = std::make_shared<HTTPClient>();
    this->output = std::make_shared<HTTPClient>();
}

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

void Task::SetPreFunc(PreFuncType _preFunc) {
    this->preFunc = _preFunc;
}
void Task::SetMainFunc(MainFuncType _mainFunc) {
    this->mainFunc = _mainFunc;
}
void Task::SetPostFunc(PostFuncType _postFunc) {
    this->postFunc = _postFunc;
}

HTTPClient& Task::GetInput() {
    return *input;
}
HTTPClient& Task::GetOutput() {
    return *output;
}

void Task::SetInput(HTTPClient& _input) {
    this->input = std::make_shared<HTTPClient>(_input);
}
void Task::SetOutput(HTTPClient& _output) {
    this->output = std::make_shared<HTTPClient>(_output);
}

bool Task::HasData() {
    return input->hasData();
}

