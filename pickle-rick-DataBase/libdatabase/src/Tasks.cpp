#include "Tasks.h"
#include <boost/any.hpp>

Tasks::Tasks(IDataBase* DB) {
	this->dataBase = DB;
}
TaskStruct Tasks::GetTask(int ID) {
	TaskStruct task;
	boost::any result;
	result = dataBase->Select("*", "Taskss", "task_id=" + to_string(ID));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		for (int i = 0; i < result_vector.size();)
		{
			task.task_id = atoi(result_vector[i++].c_str());
			task.static_id = atoi(result_vector[i++].c_str());
			task.user_id = atoi(result_vector[i++].c_str());
			task.task_file_id = atoi(result_vector[i++].c_str());
		}
		return task;
	}
	catch (boost::bad_any_cast & e)
	{
		task.task_id = -1;
		return task;
	}
}

vector<TaskStruct> Tasks::GetTasksForStatic(int staticID) {
	vector<TaskStruct> tasksList;
	vector<string> result_vector;
	result_vector = dataBase->Select("*", "Tasks", "static_id =" + to_string(staticID));
	if (result_vector.size() == 1) {
		TaskStruct task;
		task.task_id = -1;
		tasksList.push_back(task);
		return tasksList;
	}
	for (int i = 0; i < result_vector.size();)
	{
		TaskStruct task;
		task.task_id = atoi(result_vector[0].c_str());
		task.description = result_vector[1];
		task.settings = result_vector[2];
		i = i + task_field_count;
	}
	return tasksList;
}

bool Tasks::AddTask(string description, string settings) {
	string values = "(" + description + ", " + settings + ")";
	dataBase->Insert("Logs", "(user_id, static_id, path)", values);
	return true;
}

TaskStruct Tasks::CreateTask(TaskData taskData) {
	TaskStruct task;
	string values = "(" + to_string(taskData.static_id) + ", " + to_string(taskData.user_id) + ", " + to_string(taskData.task_file_id) + ")";
	dataBase->Insert("Tasks", "(static_id, user_id, task_file_id)", values);
	boost::any result;
	result = dataBase->Select("*", "Tasks", "static_id=" + to_string(taskData.static_id) + ", user_id=" + to_string(taskData.user_id) + ", task_file_id=" + to_string(taskData.task_file_id));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		for (int i = 0; i < result_vector.size();)
		{
			task.task_id = atoi(result_vector[i++].c_str());
			task.static_id = atoi(result_vector[i++].c_str());
			task.user_id = atoi(result_vector[i++].c_str());
			task.task_file_id = atoi(result_vector[i++].c_str());

		}
		return task;
	}
	catch (boost::bad_any_cast & e)
	{
		task.task_id = -1;
		return task;
	}
}