#include "Sessions.h"
#include <boost/any.hpp>

Sessions::Sessions(IDataBase* DB) {
	this->dataBase = DB;
}
SessionID Sessions::getSessionID(int ID) {
	SessionID session;
	boost::any result;
	result = dataBase->Select("*", "Sessions", "session_id=" + to_string(ID));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result);
		for (int i = 0; i < result_vector.size();)
		{
			session.session_id = atoi(result_vector[i++].c_str());
			session.user_id = atoi(result_vector[i++].c_str());
		}
		return session;
	}
	catch (boost::bad_any_cast & e)
	{
		session.session_id = -1;
		return session;
	}
}

bool Sessions::updateSessionID(SessionID session) {
	string values = "(" + to_string(session.session_id) + ", " + to_string(session.user_id) + ")";
	string condition = "user_id=" + to_string(session.user_id);
	dataBase->Update("Competitors", values, condition);
	boost::any result;
	result = dataBase->Select("*", "Users", "session_id=" + to_string(session.session_id) + ", user_name=" + to_string(session.user_id));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		return true;
	}
	catch (boost::bad_any_cast & e)
	{
		return false;
	}
}

bool Sessions::createSessionID(SessionID session) {
	SessionID newSession;
	string values = "(" + to_string(session.session_id) + ", " + to_string(session.user_id) + ")";
	dataBase->Insert("Sessions", "(session_id, user_id)", values);
	boost::any result;
	result = dataBase->Select("*", "Sessions", "session_id=" + to_string(session.session_id) + ", user_id=" + to_string(session.user_id));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		return true;
	}
	catch (boost::bad_any_cast & e)
	{
		return false;
	}
}
