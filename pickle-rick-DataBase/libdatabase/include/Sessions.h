#pragma once

#include <iostream>
#include "stdlib.h"
#include "ISessions.h"


class Sessions : public ISessions {

public:

	Sessions(IDataBase* DB);

	SessionID getSessionID(int ID) override;
	bool updateSessionID(SessionID ID) override;
	bool createSessionID(SessionID ID) override;

private:

	IDataBase* dataBase = NULL;
};