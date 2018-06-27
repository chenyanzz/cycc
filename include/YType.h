/**
 * Created by cy on 2018/5/24.
 *
 * The class of C types.
 * Now it can only work with C num types.
 */

#pragma once

#include <set>
#include <string>
#include "CYCC.h"

class YType : public CYCC 
{
public:
	YType(const YType&) = delete;
	YType(YType&&) = delete;
	YType& operator=(const YType&) = delete;
	YType& operator=(YType&&) = delete;
	virtual ~YType() = default;

protected:

	static std::set<YType*> types;

	static const char* str_unsigned;
	static const char* str_signed;


public:
	std::string name = "";
	size_t size = 0;

	enum BaseType 
	{
		cNum, cPtr, cStruct, cEnum, UNDEFINED
	};

	BaseType base_type = UNDEFINED;

protected:
	YType() = default;

public:
	typedef YType* PType;
	static PType Char, UChar, Short, UShort, Int, UInt, Long, ULong, LongLong, ULongLong, Float, Double, LongDouble;

public:
	static bool add(const char* name, int size, BaseType baseType, YType*& pNewType);

	//@throws YTypeNotFoundException
	static YType* parse(const char* code);

	static void init();

	static void terminate();

	const char* className() const override;
	void print() override;
};

