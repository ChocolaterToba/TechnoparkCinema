#pragma once

#include <functional>
#include <vector>
#include "HTTPClient.hpp"

// Main function should also set output client (maybe add list of them???).
typedef std::function<void(std::vector<char>&, HTTPClient&)> MainFuncType;
MainFuncType PreProcess(std::vector<char>& data, HTTPClient& input);
void MainProcessBasic(std::vector<char>& data, HTTPClient& output);
void PostProcess(std::vector<char>& data, HTTPClient& output);
