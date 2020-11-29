#pragma once

#include <iostream>
#include "stdlib.h"
#include "ITasks.h"
#include "IDataBase.h"
#include "DataBaseMySQL.h"

class Tasks : public ITasks {
public:
	Tasks(IDataBase* DB);

	TaskStruct GetTask(int taskID) override;

	vector<TaskStruct> GetTasksForStatic(int staticID) override;

	bool AddTask(string description, string settings) override;

	TaskStruct CreateTask(TaskData taskData) override;

private:

	IDataBase* dataBase = NULL;
};