#pragma once

#include <iostream>
#include "stdlib.h"
#include "IUsers.h"


class Users : public IUsers {

public:

	Users(IDataBase* DB);

	UserStruct getUser(UserData userData) override;
	UserStruct createUser(UserData userData) override;

private:

	IDataBase* dataBase = NULL;
};