#include "Logs.h"

Logs::Logs(IDataBase* DB) {
	this->dataBase = DB;
}
LogStruct Logs::GetLog(int logID) {
	LogStruct log;
	vector<string> result_vector;
	result_vector = dataBase->Select("*", "Logs", "id =" + to_string(logID));
	if (result_vector.size() == 1) {
		log.log_id = -1;
		return log;
	}
	for (int i = 0; i < result_vector.size();)
	{
		log.log_id = atoi(result_vector[i + 0].c_str());
		log.user_id = atoi(result_vector[i + 1].c_str());
		log.static_id = atoi(result_vector[i + 2].c_str());
		log.path = result_vector[i + 3];
		i = i + log_field_count;
	}
	return log;
}
vector<LogStruct> Logs::GetLogsForContest(int contestID) {
	vector<LogStruct> logVector;
	vector<string> result_vector;
	result_vector = dataBase->Select("*", "Logs", "static_id =" + to_string(staticID));
	if (result_vector.size() == 1) {
		LogStruct log;
		log.log_id = -1;
		logVector.push_back(log);
		return logVector;
	}
	for (int i = 0; i < result_vector.size();)
	{
		LogStruct log;
		log.log_id = atoi(result_vector[i + 0].c_str());
		log.user_id = atoi(result_vector[i + 1].c_str());
		log.static_id = atoi(result_vector[i + 2].c_str());
		log.path = result_vector[i + 3];
		i = i + log_field_count;
		logVector.push_back(log);
	}
	return logVector;
}
vector<LogStruct> Logs::GetLogsForUser(int userID) {
	vector<LogStruct> logVector;
	vector<string> result_vector;
	result_vector = dataBase->Select("*", "Logs", "static_id =" + to_string(userID));
	if (result_vector.size() == 1) {
		LogStruct log;
		log.log_id = -1;
		logVector.push_back(log);
		return logVector;
	}
	for (int i = 0; i < result_vector.size();)
	{
		LogStruct log;
		log.log_id = atoi(result_vector[i + 0].c_str());
		log.user_id = atoi(result_vector[i + 1].c_str());
		log.static_id = atoi(result_vector[i + 2].c_str());
		log.path = result_vector[i + 3];
		i = i + log_field_count;
		logVector.push_back(log);
	}
	return logVector;
}
bool Logs::AddLog(string path, int user_id, int contest_id) {
	string values = "(" + path + ", " + to_string(user_id) + ", " + to_string(user_id) + ")";
	dataBase->Insert("Logs", "(user_id, static_id, path)", values);
	return true;
}