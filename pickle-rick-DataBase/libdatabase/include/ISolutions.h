#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>

using namespace std;

struct SolutionStruct {
	int solution_id;
	int task_id;
	int log_file_id;
	int result_file_id;
};

struct SolutionData {
	int task_id;
	int log_file_id;
	int result_file_id;
};

const int task_field_count = 3;

class ISolutions {
public:
	SolutionStruct virtual GetSolution(int ID) = 0;

	SolutionStruct virtual CreateSolution(SolutionData solutionData) = 0;
};
