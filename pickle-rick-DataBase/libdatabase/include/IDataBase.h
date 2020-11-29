#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>

using namespace std;

class IDataBase {
public:
	vector<string> virtual Select(string selector, string table, string condition = "") = 0;

	vector<string> virtual Update(string table, string set, string condition = "") = 0;

	vector<string> virtual Insert(string table, string variables, string values) = 0;

	vector<string> virtual Delete_(string table, string condition) = 0;
};
