#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>
#include "IDataBase.h"
#include "DataBaseMySQL.h"

using namespace std;

struct AdminStruct {
	int admin_id;
	int static_id;
	int user_id;
};
const int admin_field_count = 3;

class IAdmins {
public:

	AdminStruct virtual GetAdmin(int adminID) = 0;

	vector<AdminStruct> virtual GetAdminsForStatic(int staticID) = 0;

	bool virtual SetAdmin(int staticID, int userID, int adminID = -1) = 0;
};