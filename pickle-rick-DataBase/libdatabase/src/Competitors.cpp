#include "Competitors.h"

CompetitorStruct Competitors::GetCompetitor(int competitorID) {
	vector<string> result_vector;
	result_vector = dataBase->Select("*", "Logs", "id =" + to_string(competitorID));
	CompetitorStruct competitor;
	if (result_vector.size() == 1) {
		competitor.competitor_id = -1;
		return competitor;
	}
	competitor.competitor_id = atoi(result_vector[0].c_str());
	competitor.static_id = atoi(result_vector[1].c_str());
	competitor.user_id = atoi(result_vector[2].c_str());
	return competitor;
}
vector<CompetitorStruct> Competitors::GetCompetitorsForStatic(int staticID) {
	vector<CompetitorStruct> competitorVector;
	vector<string> result_vector;
	result_vector = dataBase->Select("*", "Competitors", "static_id =" + to_string(staticID));
	if (result_vector.size() == 1) {
		CompetitorStruct competitor;
		competitor.competitor_id = -1;
		competitorVector.push_back(competitor);
		return competitorVector;
	}
	for (int i = 0; i < result_vector.size();)
	{
		CompetitorStruct competitor;
		competitor.competitor_id = atoi(result_vector[i + 0].c_str());
		competitor.static_id = atoi(result_vector[i + 1].c_str());
		competitor.user_id = atoi(result_vector[i + 2].c_str());
		i = i + competitor_field_count;
		competitorVector.push_back(competitor);
	}
	return competitorVector;
}
bool Competitors::SetCompetitor(int staticID, int userID, int competitorID) {
	if (competitorID != -1) {
		string values = "(" + to_string(staticID) + ", " + to_string(userID) + ")";
		string condition = "competitor_id=" + to_string(competitorID);
		dataBase->Update("Competitors", values, condition);
	}
	else {
		string values = "(" + to_string(staticID) + ", " + to_string(userID) + ")";
		dataBase->Insert("Competitors", "(static_id, user_id)", values);
	}
	return true;
}