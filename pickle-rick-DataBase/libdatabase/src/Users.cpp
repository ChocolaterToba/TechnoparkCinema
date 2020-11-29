#include "Users.h"
#include <boost/any.hpp>
Users::Users(IDataBase* DB) {
	this->dataBase = DB;
}
UserStruct Users::getUser(UserData userData) {
	UserStruct user;
	boost::any result;
	result = dataBase->Select("*", "Users", "user_password=" + userData.user_password + ", user_name=" + userData.user_name);
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		for (int i = 0; i < result_vector.size();)
		{
			user.user_id = atoi(result_vector[i++].c_str());
			user.user_name = atoi(result_vector[i++].c_str());
			user.user_password = atoi(result_vector[i++].c_str());
		}
		return user;
	}
	catch (boost::bad_any_cast & e)
	{
		user.user_id = -1;
		return user;
	}
}

UserStruct Users::createUser(UserData userData) {
	UserStruct user;
	string values = "(" + userData.user_name + ", " + userData.user_password + ")";
	dataBase->Insert("Users", "(user_id, user_username, user_password)", values);
	boost::any result;
	result = dataBase->Select("*", "Users", "user_password=" + userData.user_password + ", user_name=" + userData.user_name);
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		for (int i = 0; i < result_vector.size();)
		{
			user.user_id = atoi(result_vector[i++].c_str());
			user.user_name = atoi(result_vector[i++].c_str());
			user.user_password = atoi(result_vector[i++].c_str());
		}
		return user;
	}
	catch (boost::bad_any_cast & e)
	{
		user.user_id = -1;
		return user;
	}
}
