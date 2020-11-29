#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>

using namespace std;

struct TaskStruct {
	int task_id;
	int user_id;
	int static_id;
	int task_file_id;
};

struct TaskData {
	int user_id;
	int static_id;
	int task_file_id;
};

const int task_field_count = 3;

class ITasks {
public:
	TaskStruct virtual GetTask(int ID) = 0;

	vector<TaskStruct> virtual GetTasksForStatic(int staticID) = 0;

	bool virtual AddTask(string description, string settings) = 0;

	TaskStruct virtual CreateTask(TaskData taskData) = 0;
};
