#pragma once

#include "DataBaseMySQL.h"

DataBaseMySQL::DataBaseMySQL() : conn(mysql_init(NULL)) {
	conn = mysql_real_connect(conn, "localhost", "root", "36360326v", "testdb", 3306, NULL, 0);
	if (!conn) {
		cout << "Connection Failed" << endl;
	}
}
vector<string> DataBaseMySQL::Select(string selector, string table, string condition) {

	result_vector.clear();
	if (!conn)
	{
		result_vector.push_back("There is no connection");
		return result_vector;
	}
	form_select_query(selector, table, condition);
	if (!qstate)
	{
		take_result_from_select();
	}
	else
	{
		result_vector.push_back("Query failed: " + mysql_errno(conn));
	}
	return result_vector;
}

vector<string> DataBaseMySQL::Update(string table, string set, string condition) {
	result_vector.clear();
	if (!conn)
	{
		result_vector.push_back("There is no connection");
		return result_vector;
	}
	form_update_query(table, set, condition);
	if (!qstate)
	{
		result_vector.push_back("Update Success:");
	}
	else
	{
		result_vector.push_back("Update failed: " + mysql_errno(conn));
	}
	return result_vector;
}

vector<string> DataBaseMySQL::Insert(string table, string variables, string values) {
	result_vector.clear();
	if (!conn)
	{
		result_vector.push_back("There is no connection");
		return result_vector;
	}
	form_insert_query(table, variables, values);
	if (!qstate)
	{
		result_vector.push_back("Insert Success:");
	}
	else
	{
		result_vector.push_back("Insert failed: " + mysql_errno(conn));
	}
	return result_vector;
}

vector<string> DataBaseMySQL::Delete_(string table, string condition) {
	result_vector.clear();
	if (!conn)
	{
		result_vector.push_back("There is no connection");
		return result_vector;
	}
	form_delete_query(table, condition);
	if (!qstate)
	{
		result_vector.push_back("Insert Success:");
	}
	else
	{
		result_vector.push_back("Insert failed: " + mysql_errno(conn));
	}
	return result_vector;
}

// private

void DataBaseMySQL::form_select_query(string selector, string table, string condition) {
	query = "";
	if (condition != "") {
		query = "SELECT " + selector + " FROM " + table + " WHERE " + condition + ";";
	}
	else {
		query = "SELECT " + selector + " FROM " + table + ";";
	}
	const char* cquery = query.c_str();
	qstate = mysql_query(conn, cquery);
}

void DataBaseMySQL::form_update_query(string table, string set, string condition) {
	query = "";
	if (condition != "") {
		query = "UPDATE " + table + " SET " + set + " WHERE " + condition + ";";
	}
	else {
		query = "UPDATE " + table + " SET " + set + ";";
	}
	const char* cquery = query.c_str();
	qstate = mysql_query(conn, cquery);
}

void DataBaseMySQL::form_insert_query(string table, string variables, string values) {
	query = "INSERT INTO " + table + " " + variables + " VALUES " + values + ";";
	const char* cquery = query.c_str();
	qstate = mysql_query(conn, cquery);
}

void DataBaseMySQL::form_delete_query(string table, string condition) {
	query = "DELETE FROM " + table + " WHERE " + condition + ";";
	const char* cquery = query.c_str();
	qstate = mysql_query(conn, cquery);
}

void DataBaseMySQL::take_result_from_select() {
	res = mysql_store_result(conn);
	while (row = mysql_fetch_row(res))
	{
		for (int i = 0; i < mysql_num_fields(res); i++)
		{
			result_vector.push_back(row[i]);
		}
	}
}
