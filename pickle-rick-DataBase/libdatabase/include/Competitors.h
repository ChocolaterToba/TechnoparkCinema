#pragma once

#include <iostream>
#include "stdlib.h"
#include "ICompetitors.h"
#include "IDataBase.h"
#include "DataBaseMySQL.h"

class Competitors : public ICompetitors {
public:
	Competitors(IDataBase* DB) {
		this->dataBase = DB;
	}

	CompetitorStruct 

	CompetitorStruct GetCompetitor(int competitorID) override;

	vector<CompetitorStruct> GetCompetitorsForStatic(int staticID) override;

	bool virtual SetCompetitor(int statictID, int userID, int competitorID = -1) override;

private:

	IDataBase* dataBase = NULL;

};