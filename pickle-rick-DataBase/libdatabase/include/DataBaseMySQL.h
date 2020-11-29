#pragma once

#include <iostream>
#include "stdlib.h"
#include <string>
#include <vector>
#include "IDataBase.h"
#include "mysql.h"

class DataBaseMySQL : public IDataBase {

public:
	DataBaseMySQL();

	vector<string> Select(string selector, string table, string condition = "") override;

	vector<string> Update(string table, string set, string condition = "") override;

	vector<string> Insert(string table, string variables, string values) override;

	vector<string> Delete_(string table, string condition) override ;

private:
	void form_select_query(string selector, string table, string condition = "");

	void form_update_query(string table, string set, string condition = "");

	void form_insert_query(string table, string variables, string values);

	void form_delete_query(string table, string condition);

	void take_result_from_select();

	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	vector<string> result_vector;
	string query;
	int qstate;
};