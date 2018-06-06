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

class YVal :public CYCC {
public:
    void print() override;
    const char* className() const override;

    static bool parse(char* s, YVal*& pVal);

    YVal* clone();
    YVal* castTo(YType* pNewType);

    ~YVal();

protected:
    YType* pType;//pType is destroyed when going out of scope.
    byte* pData;

	//NOTE that pVal must be available
    static bool parseInt(const char* s,YVal* pVal);
    static bool parseDecimal(const char* s,YVal* pVal);


    YVal() : pType(nullptr), pData(nullptr) {};

    YVal(YType* ptype, void* pdata);
    YVal(YVal* pVal);


    friend void test_YVal();//for debug
};



