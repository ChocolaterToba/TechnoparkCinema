#pragma once

#include <iostream>
#include "stdlib.h"
#include <vector>
#include "IDataBase.h"
#include "DataBaseMySQL.h"

using namespace std;

struct SessionID {
	int user_id;
	int session_id;
};

const int static_field_count = 4;

class ISessions {
public:
	SessionID virtual getSessionID(int ID) = 0;
	bool virtual updateSessionID(SessionID session) = 0;
	bool virtual createSessionID(SessionID session) = 0;
};