#pragma once

/**
 * Created by cy on 2018/6/1.
 */

#include <string>
#include "YVal.h"

class YVar {
protected:
	const std::string name = "";
	YVal* pVal             = nullptr;

public:
	YVar(const char* name) : name(name) {};

	void setVal(YVal* pVal);
};
