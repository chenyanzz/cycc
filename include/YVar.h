#pragma once

/**
 * Created by cy on 2018/6/1.
 */

#include <string>
#include "YVal.h"

class YVar {
protected:
    const std::string name;
    YVal* pVal;

public:
    YVar(const char* __name) : name(__name), pVal(nullptr) {};

    void setVal(YVal* pVal);
};