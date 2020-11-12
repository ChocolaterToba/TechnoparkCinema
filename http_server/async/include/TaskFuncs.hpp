#pragma once

#include <vector>
#include <functional>
#include "HTTPClient.hpp"

typedef std::function<void(std::vector<char>&)> MainFuncType;
MainFuncType PreProcess(vector<char>& data, HTTPClient& client);
void MainProcessBasic(std::vector<char>& data);
void PostProcess(std::vector<char>& data, HTTPClient& client);
