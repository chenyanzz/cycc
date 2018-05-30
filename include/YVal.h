#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * The class for saving values.
 */

#include "YType.h"

class YVal : CYCC {
public:
    void print() override;
    const char* className() const override;
    static YVal* parse(char* s);

    ~YVal();

protected:
    YType* ptype;//ptype is destroyed when going out of scope.
    void* pdata;

    static YVal* parseInt(const char* s);
    static YVal* parseDecimal(const char* s);

    YVal() = default;
    friend void test_YVal();//for debug
};



