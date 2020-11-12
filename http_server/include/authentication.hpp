#pragma once
#include <string>
#include <mysql.h>

class SessionManager {
 public:
    static SessionManager* Instance();
    static SessionManager* Create(unsigned int SessionTimeout, unsigned int ServerTimeout);
    
    ~SessionManager();

    unsigned long long GenerateSession(const char* AccountName, signed long long UID);
    void WipeSessions();
    void WipeServers();
    void RemoveExpiredSessions();
    void RemoveExpiredServers();

    unsigned int SessionTimeout;
    unsigned int ServerTimeout;

 protected:
    SessionManager(unsigned int SessionTimeout, unsigned int ServerTimeout);

 private:
    static SessionManager* Pointer;

    unsigned long long GenerateUniqueKey();
};

class DBManager {
 public:
    static DBManager* Instance();
    static DBManager* Create(const char* dbHost, unsigned int dbPort, const char* dbUser, const char* dbPass, const char* dbName);
    
    ~DBManager();

    bool Query(const char* query);
    MYSQL_RES* StoreResult();
    int NumRows(MYSQL_RES* result);
    MYSQL_ROW FetchRow(MYSQL_RES* result);
    void FreeResult(MYSQL_RES* result);
    int ValidatePlayer(const char* Name, const char* Password, signed long long* UID);

 protected:
    DBManager(const char* dbHost, unsigned int dbPort, const char* dbUser, const char* dbPass, const char* dbName);

 private:
    static DBManager* Pointer;

    MYSQL*				dbHandle;
    std::string			dbHost;
    unsigned int		dbPort;
    std::string			dbUser;
    std::string			dbName;
    std::string			dbPass;
    std::string			UsersTable;
};
