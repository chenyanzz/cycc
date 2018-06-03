#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * The class for saving values.
 * such as constant "1ll"
 * or as the ans of (i+1)
 */

#include "YType.h"
#include "YException.h"

class YVal : CYCC {
public:
    void print() override;
    const char* className() const override;
    static YVal* parse(char* s);

    YVal* clone();
    YVal* castTo(YType* pNewType);

    ~YVal();

protected:
    YType* ptype;//ptype is destroyed when going out of scope.
    byte* pdata;

    static YVal* parseInt(const char* s);
    static YVal* parseDecimal(const char* s);

    YVal() : ptype(nullptr), pdata(nullptr) {};

    YVal(YType* ptype,void* pdata);
    YVal(YVal* pVal);

    friend void test_YVal();//for debug
};



