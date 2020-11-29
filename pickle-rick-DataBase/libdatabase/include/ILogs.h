#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>

using namespace std;

struct LogStruct {
	int log_id;
	int user_id;
	int static_id;
	string path;
};
const int log_field_count = 4;

class ILogs {
public:
	LogStruct virtual GetLog(int logID) = 0;

	vector<LogStruct> virtual GetLogsForStatic(int staticID) = 0;

	vector<LogStruct> virtual GetLogsForUser(int userID) = 0;

	bool virtual AddLog(string path, int user_id, int static_id) = 0;
};