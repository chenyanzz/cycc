#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * The class for saving values.
 */

#include "YType.h"

class YVal : CYCC {
public:
    static YVal parse(char* s);

    void print() override ;

    ~YVal();

protected:
    YType* ptype;//ptype is destroyed when going out of scope.
    void* pdata;

    static YVal* parseInt(const char* s, bool* bSuccess);

    friend void test_YVal();//for debug

    YVal() = default;
};



