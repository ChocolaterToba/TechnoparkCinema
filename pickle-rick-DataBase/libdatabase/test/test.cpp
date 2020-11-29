#include "gtest/gtest.h"
#include "mysql.h"

class TestDataBase : public IDataBase {
public:
	TestDataBase() : conn(mysql_init(NULL)) {
		conn = mysql_real_connect(conn, "localhost", "root", "36360326v", "testdb", 3306, NULL, 0);
		if (!conn) {
			cout << "Connection Failed" << endl;
		}
	}
	vector<string> select(string selector, string table, string condition) {

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

	vector<string> update(string table, string set, string condition) {
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

	vector<string> insert(string table, string variables, string values) {
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

	vector<string> delete_(string table, string condition) {
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

private:

	void form_select_query(string selector, string table, string condition) {
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

	void form_update_query(string table, string set, string condition) {
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

	void form_insert_query(string table, string variables, string values) {
		query = "INSERT INTO " + table + " " + variables + " VALUES " + values + ";";
		const char* cquery = query.c_str();
		qstate = mysql_query(conn, cquery);
	}

	void form_delete_query(string table, string condition) {
		query = "DELETE FROM " + table + " WHERE " + condition + ";";
		const char* cquery = query.c_str();
		qstate = mysql_query(conn, cquery);
	}

	void take_result_from_select() {
		res = mysql_store_result(conn);
		while (row = mysql_fetch_row(res))
		{
			for (int i = 0; i < mysql_num_fields(res); i++)
			{
				result_vector.push_back(row[i]);
			}
		}
	}

	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;
	vector<string> result_vector;
	string query;
	int qstate;
};

struct staticDB {

    staticDB() {
		DB = new  TestDataBase;
		admins = new Admins(DB);
		competitors = new Competitors(DB);
		tasks = new Tasks(DB);
		logs = new Logs(DB);
        statics = new Statics(admins, competitors, logs, tasks, DB);
	};

	TestDataBase* DB;
	Admins* admins;
	Competitors* competitors;
	Tasks* tasks;
	Logs* logs;
    Statics* statics;
};

/* Тестирование создания объектов. */

TEST(TestCreate, CreateAdmins) {
	TestDataBase base;
	Admins* admins = NULL;
	admins = new Admins(&base);
	EXPECT_TRUE(admins != NULL);
}

TEST(TestCreate, CreateCompetitors) {
	TestDataBase base;
	Competitors* competitors = NULL;
	competitors = new Competitors(&base);
	EXPECT_TRUE(competitors != NULL);
}

TEST(TestCreate, CreateLogs) {
	TestDataBase base;
	Logs* logs = NULL;
	logs = new Logs(&base);
	EXPECT_TRUE(logs != NULL);
}

TEST(TestCreate, CreateTasks) {
	TestDataBase base;
	Tasks* tasks = NULL;
	tasks = new Tasks(&base);
	EXPECT_TRUE(tasks != NULL);
}

TEST(TestCreate, CreateStatics) {
	TestDataBase base;
    Statics* statics = NULL;
    statics = new Statics(NULL,NULL,NULL,NULL,&base);
	EXPECT_TRUE(statics != NULL);
}

TEST(TestCreate, CreateDataBase) {
	DataBaseMySQL* base = new DataBaseMySQL;
	EXPECT_TRUE(base != NULL);
}

/* Тестирование Admins */

TEST(TestAdmins, TestGetAdminsForStatic) {
staticDB* base = new staticDB();
	unsigned int staticID = 3;
	vector<AdminStruct> adminsList = base->admins->getAdminsForStatic(staticID);
	ASSERT_TRUE(adminsList.size() > 0);
}

TEST(TestAdmins, TestGetAdmin) {
staticDB* base = new staticDB();
	unsigned int adminID = 42;
	AdminStruct admin;
	admin = base->admins->getAdmin(adminID);
	ASSERT_TRUE(admin.admin_id < 0);
}


TEST(TestAdmins, TestAddAdmin) {
staticDB* base = new staticDB();
	unsigned int userID = 10;
	unsigned int staticID = 3;
	bool result = base->admins->setAdmin(staticID, userID);
	ASSERT_TRUE(result);
}

/* Тестирование Competitors */

TEST(TestCompetitors, TestGetCompetitorsForStatic) {
staticDB* base = new staticDB();
	unsigned int staticID = 4;
	vector<CompetitorStruct> competitorslList = base->competitors->getCompetitorsForStatic(staticID);
	ASSERT_TRUE(!competitorslList.empty());
}

TEST(TestCompetitors, TestGetCompetitor) {
staticDB* base = new staticDB();
	unsigned int competitorID = 4;
	CompetitorStruct competitor = base->competitors->getCompetitor(competitorID);
	ASSERT_TRUE(competitor.competitor_id < 0);
}

TEST(TestCompetitors, TestSetCompetitor) {
staticDB* base = new staticDB();
	int static_id = 4;
	int user_id = 2;
	bool result = base->competitors->setCompetitor(static_id, user_id);
	ASSERT_TRUE(result);
}


/* Тестирование Logs */

TEST(TestLogs, TestGetLog) {
staticDB* base = new staticDB();
	unsigned int logID = 9;
	LogStruct log = base->logs->getLog(logID);
	ASSERT_TRUE(log.static_id != -1);
}

TEST(TestLogs, TestGetLogsForStatic) {
staticDB* base = new staticDB();
	unsigned int staticID = 4;
	vector<LogStruct> logsList = base->logs->getLogsForStatic(staticID);
	ASSERT_TRUE(!logsList.empty());
}

TEST(TestLogs, TestGetLogsForUser) {
staticDB* base = new staticDB();
	unsigned int userID = 33;
	vector<LogStruct> logsList = base->logs->getLogsForUser(userID);
	ASSERT_TRUE(!logsList.empty());
}

TEST(TestLogs, TestAddLog) {
staticDB* base = new staticDB();
	int user_id = 12;
	int static_id = 4;
	string path;
	bool result = base->logs->addLog(path, user_id, static_id);
	ASSERT_TRUE(result);
}

/* Тестирование Tasks */

TEST(TestTasks, TestGetTasksForStatic) {
staticDB* base = new staticDB();
	unsigned int staticID = 6;
	vector<TaskStruct> tasksList = base->tasks->getTasksForStatic(staticID);
	ASSERT_TRUE(!tasksList.empty());
}

TEST(TestTasks, TestGetTask) {
staticDB* base = new staticDB();
	unsigned int taskID = 6;
	TaskStruct task = base->tasks->getTask(taskID);
	ASSERT_TRUE(task.task_id < 0);
}

TEST(TestTasks, TestSetTask) {
    staticDB* base = new staticDB();
	string description = "[{}]";
	string settings = "[{}]";
	bool result = base->tasks->addTask(description, settings);
	ASSERT_TRUE(result);
}
//* Тестирование Statics */
