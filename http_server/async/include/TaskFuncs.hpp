#pragma once

#include <functional>
#include <vector>
#include <map>
#include <string>
#include <mutex>
#include "HTTPClient.hpp"

using std::vector;
using std::string;
using std::map;

typedef std::function<void(map<string, string>&, vector<char>&,
                           map<int, HTTPClient>&, std::shared_ptr<std::mutex>,
                           HTTPClient&, HTTPClient&)> MainFuncType;

MainFuncType PreProcess(map<string, string>& headers,
                        vector<char>& data,
                        HTTPClient& input);

void MainProcessBasic(map<string, string>& headers,
                      vector<char>& body,
                      map<int, HTTPClient>& pendingDBResponse,
                      std::shared_ptr<std::mutex> pendingDBResponseMutex,
                      HTTPClient& input, HTTPClient& output);

void MainProcessDBServer(map<string, string>& headers,
                         vector<char>& body,
                         map<int, HTTPClient>& pendingDBResponse,
                         std::shared_ptr<std::mutex> pendingDBResponseMutex,
                         HTTPClient& input, HTTPClient& output);

void MainProcessDBReceived(map<string, string>& headers,
                           vector<char>& body,
                           map<int, HTTPClient>& pendingDBResponse,
                           std::shared_ptr<std::mutex> pendingDBResponseMutex,
                           HTTPClient& input, HTTPClient& output);

void PostProcess(map<string, string>& headers,
                 vector<char>& body,
                 HTTPClient& output);
