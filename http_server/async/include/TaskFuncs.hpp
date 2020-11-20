#pragma once

#include <functional>
#include <vector>
#include "HTTPClient.hpp"

typedef std::function<void(std::vector<char>&)> MainFuncType;
MainFuncType PreProcess(std::vector<char>& data, HTTPClient& client);
void MainProcessBasic(std::vector<char>& data);
void PostProcess(std::vector<char>& data, HTTPClient& client);
