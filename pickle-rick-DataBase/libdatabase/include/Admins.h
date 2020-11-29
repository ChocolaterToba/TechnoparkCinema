#pragma once

#include <iostream>
#include "stdlib.h"
#include "IAdmins.h"


class Admins : public IAdmins {

public:

	Admins(IDataBase* Base) {
		this->dataBase = Base;
	}



	AdminStruct GetAdmin(int adminID) override;

	vector<AdminStruct> GetAdminsForStatic(int staticID) override;

	bool SetAdmin(int staticID, int userID, int adminID = -1) override;


private:

	IDataBase* dataBase = NULL;
};