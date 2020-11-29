#include "Solutions.h"
#include <boost/any.hpp>

Solutions::Solutions(IDataBase* DB) {
	this->dataBase = DB;
}
SolutionStruct Solutions::GetSolution(int ID) {
	SolutionStruct solution;
	boost::any result;
	result = dataBase->Select("*", "Solutions", "solution_ID=" + to_string(ID));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result);
		for (int i = 0; i < result_vector.size();)
		{
			solution.solution_id = atoi(result_vector[i++].c_str());
			solution.task_id = atoi(result_vector[i++].c_str());
			solution.log_file_id = atoi(result_vector[i++].c_str());
			solution.result_file_id = atoi(result_vector[i++].c_str());
		}
		return solution;
	}
	catch (boost::bad_any_cast & e)
	{
		solution.solution_id = -1;
		return solution;
	}
}

SolutionStruct Solutions::CreateSolution(SolutionData solutionData) {
	SolutionStruct newSolution;
	string values = "(" + to_string(solutionData.task_id) + ", " + to_string(solutionData.log_file_id) + ", " + to_string(solutionData.result_file_id) + ")";
	dataBase->Insert("Solutions", "(task_id, log_file_id, result_file_id)", values);
	boost::any result;
	result = dataBase->Select("*", "Solutionss", "log_file_id=" + to_string(solutionData.log_file_id) + ", result_file_id=" + to_string(solutionData.result_file_id));
	try
	{
		vector<string> result_vector = boost::any_cast<vector<string>>(result_vector);
		for (int i = 0; i < result_vector.size();)
		{
			newSolution.solution_id = atoi(result_vector[i++].c_str());
			newSolution.task_id = atoi(result_vector[i++].c_str());
			newSolution.log_file_id = atoi(result_vector[i++].c_str());
			newSolution.result_file_id = atoi(result_vector[i++].c_str());
		}
		return newSolution;
	}
	catch (boost::bad_any_cast & e)
	{
		newSolution.solution_id = -1;
		return newSolution;
	}
}
