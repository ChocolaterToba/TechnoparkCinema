#include "Admins.h"
#include <boost/any.hpp>

AdminStruct Admins::GetAdmin(int adminID) {
	AdminStruct admin;
	boost::any result_vector;
	result_vector = dataBase->Select("*", "Admins", "id =" + to_string(adminID));
	try 
	{
		vector<string> result = boost::any_cast<vector<string>>(result_vector);
		int i = 0;
		admin.admin_id = atoi(result[i++].c_str());
		admin.static_id = atoi(result[i++].c_str());
		admin.user_id = atoi(result[i].c_str());
		return admin;
	}
	catch (boost::bad_any_cast & e)
	{
		admin.admin_id = -1;
		return admin;
	}
}

vector<AdminStruct> Admins::GetAdminsForStatic(int staticID) {
	vector<AdminStruct> AdminVector;
	boost::any result;
	result = dataBase->Select("*", "Admins", "static_id =" + to_string(staticID));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result);
		for (int i = 0; i < result_vector.size();)
		{
			AdminStruct admin;
			admin.admin_id = atoi(result_vector[i++].c_str());
			admin.static_id = atoi(result_vector[i++].c_str());
			admin.user_id = atoi(result_vector[i++].c_str());
			AdminVector.push_back(admin);
		}
		return AdminVector;
		
	}
	catch (boost::bad_any_cast & e)
	{
		AdminStruct admin;
		admin.admin_id = -1;
		AdminVector.push_back(admin);
		return AdminVector;
	}
}

bool  Admins::SetAdmin(int staticID, int userID, int adminID) {
	if (adminID != -1) {
		string values = "(" + to_string(staticID) + ", " + to_string(userID) + ")";
		string condition = "admin_id=" + to_string(adminID);
		dataBase->Update("Admins", values, condition);
	}
	else {
		string values = "(" + to_string(staticID) + ", " + to_string(userID) + ")";
		dataBase->Insert("Admins", "(static_id, user_id)", values);
	}
	return true;
}