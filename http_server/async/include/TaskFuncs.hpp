#pragma once

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include "HTTPClient.hpp"

typedef std::function<void(std::map<std::string, std::string>&, std::vector<char>&,
                           std::map<int, HTTPClient>&, std::shared_ptr<std::mutex>,
                           HTTPClient&, HTTPClient&)> MainFuncType;
MainFuncType PreProcess(std::map<std::string, std::string>& headers,
                        std::vector<char>& data,
                        HTTPClient& input);
void MainProcessBasic(std::map<std::string, std::string>& headers,
                      std::vector<char>& body,
                      std::map<int, HTTPClient>& pendingDBResponse,
                      std::shared_ptr<std::mutex> pendingDBResponseMutex,
                      HTTPClient& input, HTTPClient& output);
void MainProcessDBServer(std::map<std::string, std::string>& headers,
                         std::vector<char>& body,
                         std::map<int, HTTPClient>& pendingDBResponse,
                         std::shared_ptr<std::mutex> pendingDBResponseMutex,
                         HTTPClient& input, HTTPClient& output);
void MainProcessDBReceived(std::map<std::string, std::string>& headers,
                           std::vector<char>& body,
                           std::map<int, HTTPClient>& pendingDBResponse,
                           std::shared_ptr<std::mutex> pendingDBResponseMutex,
                           HTTPClient& input, HTTPClient& output);
void PostProcess(std::map<std::string, std::string>& headers,
                 std::vector<char>& body,
                 HTTPClient& output);
