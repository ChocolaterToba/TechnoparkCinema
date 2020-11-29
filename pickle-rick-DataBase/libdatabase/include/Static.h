#pragma once

#include <iostream>
#include "stdlib.h"
#include "IContest.h"
#include "Competitors.h"
#include "Logs.h"
#include "Tasks.h"
#include "Admins.h"
#include "IDataBase.h"
#include "DataBaseMySQL.h"



class Statics : public IStatic {
public:
    StaticData(Admins* admins, Competitors* competitors, Logs* logs, Tasks* tasks, IDataBase* DB);

	StaticStruct GetStatic(int staticID) override;

	StaticStruct GetStatic(string title) override;

	vector<SaticStruct> GetStaticsList() override;

	bool SetStatic(int start_time, int end_time, string description) override;

    StaticStruct CreateStatic(StaticData staticData) override;

private:

	IDataBase* dataBase = NULL;

};