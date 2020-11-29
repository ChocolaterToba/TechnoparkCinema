#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>

using namespace std;

struct StaticStruct {
	int static_id;
	int user_id;
	int input_file_id;
	int output_file_id;
	int description_file_id;
};

struct StaticData {
	int user_id;
	int input_file_id;
	int output_file_id;
	int description_file_id;
};
const int static_field_count = 4;

class IStatic {
public:
    StatictStruct virtual GetStatic(int staticID) = 0;

    StaticStruct virtual GetStatic(string title) = 0;

	vector<StaticStruct> virtual GetStaticsList() = 0;

	bool virtual SetStatic(int start_time, int end_time, string description) = 0;

    StaticStruct virtual CreateStatic (StaticData staticData) = 0;
};