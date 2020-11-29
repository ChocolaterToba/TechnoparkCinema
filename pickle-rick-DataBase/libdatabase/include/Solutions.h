#pragma once

#include <iostream>
#include "stdlib.h"
#include "ISolutions.h"
#include "IDataBase.h"
#include "DataBaseMySQL.h"

class Solutions : public ISolutions {
public:
	Solutions(IDataBase* DB);

	SolutionStruct GetSolution(int ID) override;

	SolutionStruct CreateSolution(SolutionData solutionData) override;

private:

	IDataBase* dataBase = NULL;
};