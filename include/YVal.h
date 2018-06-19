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
#include "common.h"
#include "Excutable.h"

class YVal : public Executable
{
public:
	YVal(const YVal&) = delete;
	YVal(YVal&&) = delete;
	YVal& operator=(const YVal&) = delete;
	YVal& operator=(YVal&&) = delete;

	static bool parse(const char* s, YVal*& pVal);

    YVal* clone();
    YVal* castTo(YType* pNewType);

	template<typename Type=void> 
	Type*& data();

	YType* type();

	YVal* execute() override;

	//operators below

	static YVal* add(YVal* v1, YVal* v2);
	static YVal* sub(YVal* v1, YVal* v2);
	static YVal* mul(YVal* v1, YVal* v2);
	static YVal* div(YVal* v1, YVal* v2);

	~YVal();
    friend void test_YVal();//for debug
	friend class YExpression;
protected:

	YType* pType=nullptr;//can NOT be deleted when destructing
    byte* pData=nullptr;//pData is auto-destroyed

	//NOTE that pVal must be available
    static bool parseInt(const char* s,YVal* pVal);
    static bool parseDecimal(const char* s,YVal* pVal);


    YVal()=default;

    YVal(YType* ptype, void* pdata);
    YVal(YVal* pVal);

public:
	void print() override;
	const char* className() const override;
};
