#pragma once

#include <vector>
#include <functional>
#include "HTTPClient.hpp"

std::function<void(std::vector<char>&)> PreProcess(vector<char>& data,
                                                   HTTPClient& client);
void MainProcessEmpty(std::vector<char>& data, HTTPClient& client);
void PostProcess(std::vector<char>& data, HTTPClient& client);
