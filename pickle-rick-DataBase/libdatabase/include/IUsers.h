#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>
#include "IDataBase.h"
#include "DataBaseMySQL.h"

using namespace std;

struct UserStruct {
	int user_id;
	std::string user_password;
	std::string user_name;
};

struct UserData {
	string user_password;
	string user_name;
};
const int static_field_count = 4;

class IUsers {
public:
	UserStruct virtual getUser(UserData) = 0;
	UserStruct virtual createUser(UserData) = 0;
};