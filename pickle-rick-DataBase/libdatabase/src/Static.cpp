#include "Static.h"
#include <boost/any.hpp>

Statics::Statics(Admins* admins, Competitors* competitors, Logs* logs, Tasks* tasks, IDataBase* DB) {
	this->dataBase = DB;
}
StaticsStruct Statics::GetStatics(int staticID) {

}
StaticsStruct Statics::GetStatics(string title) {

}
vector<StaticsStruct> Statics::GetStaticsList() {

	return staticaVector;
}
bool Statics::SetStatics(int start_time, int end_time, string description) {
	string values = "(" + to_string(start_time) + ", " + to_string(end_time) + ", " + description + ")";
	dataBase->Insert("Statics", "(stert_time, end_time, description)", values);
	return true;
}

StaticsStruct Statics::CreateStatics(StaticsData contestData) {

}