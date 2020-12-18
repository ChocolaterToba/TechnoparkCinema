#pragma once

#include <functional>
#include <vector>
#include "HTTPClient.hpp"

// Main function should also set output client (maybe add list of them???).
typedef std::function<void(std::string&, std::vector<char>&, HTTPClient&)> MainFuncType;
MainFuncType PreProcess(std::string& headers, std::vector<char>& data, HTTPClient& input);
void MainProcessBasic(std::string& headers, std::vector<char>& body, HTTPClient& output);
void PostProcess(std::string& headers, std::vector<char>& body, HTTPClient& output);
