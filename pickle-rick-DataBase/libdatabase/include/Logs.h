#pragma once

#include <iostream>
#include "stdlib.h"
#include "ILogs.h"
#include "IDataBase.h"
#include "DataBaseMySQL.h"


class Logs : public ILogs {
public:
	Logs(IDataBase* DB);

	LogStruct GetLog(int logID) override;

	vector<LogStruct> GetLogsForStatic(int staticID) override;

	vector<LogStruct> GetLogsForUser(int userID) override;

	bool AddLog(string path, int user_id, int static_id) override;

private:

	IDataBase* dataBase = NULL;

};