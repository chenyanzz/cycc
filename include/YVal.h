#pragma once

/*
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
#include "YVar.h"

class YVal: public Executable {
public:

	//@throws YParseFailedException
	static YVal* parse(const char* s);

	YVal* clone();
	YVal* castTo(YType* pNewType);

	template <typename Type = void>
	Type& data();

	YType* type();

	YVal* execute() override;

	//operators below

	static YVal* add(YVal* v1, YVal* v2);
	static YVal* sub(YVal* v1, YVal* v2);
	static YVal* mul(YVal* v1, YVal* v2);
	static YVal* div(YVal* v1, YVal* v2);

	static YVal* neg(YVal* v);

	virtual ~YVal();
	friend void test_YVal(); //for debug
	friend class YExpression;
	friend class YVar;

protected:
	YType* pType; //can NOT be deleted when destructing
	byte* pData = nullptr; //pData is auto-destroyed

protected:
	//!@throws YInvalidCharException
	static YVal* parseInt(const char* s);
	//!@throws YInvalidCharException
	static YVal* parseDecimal(const char* s);

	YVal() = default;

	YVal(YType* ptype);
	YVal(YType* ptype, void* pdata);
	YVal(YVal* pVal);

	//!@param operation : a lambda (or sth. else) like: [](auto v1,auto v2){return v1+v2;}	\n
	//!for current it is called with and returns (long double)type.
	template <typename func_2op_t>
	static YVal* exec_op2(YVal* v1, YVal* v2, func_2op_t operation);

	//!@param operation : a lambda (or sth. else) like: [](auto v){return -v;}	\n
	//!for current it is called with and returns (long double)type.
	template <typename func_1op_t>
	static YVal* exec_op1(YVal* v, func_1op_t operation);
public:
	void print() override;
	const char* className() const override;
};
