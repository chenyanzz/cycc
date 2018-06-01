#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * The class for saving values.
 * such as constant "1ll"
 * or as the ans of (i+1)
 */

#include "YType.h"

class YVal : CYCC {
public:
    void print() override;
    const char* className() const override;
    static YVal* parse(char* s);

    //to clone a val
    //NOT IMPLEMENTED
    YVal(YVal* pVal);
    YVal(YVal* pVal,YType* pNewType);

    ~YVal();

protected:
    YType* ptype;//ptype is destroyed when going out of scope.
    void* pdata;

    static YVal* parseInt(const char* s);
    static YVal* parseDecimal(const char* s);

    YVal() : ptype(nullptr), pdata(nullptr) {};


    YVal(YType* type, void* data) : ptype(type), pdata(data) {};

    friend void test_YVal();//for debug
};



