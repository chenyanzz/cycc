/*
 * Created by cy on 2018/5/24.
 *
 * The class of C types.
 * Now it can only work with C num types.
 */

#pragma once

#include <set>
#include <string>
#include "CYCC.h"

class YType: public CYCC {
public:
	virtual ~YType() = default;

protected:
	typedef std::set<YType*> TypeList;
	static TypeList types;
	
public:
	std::string name = "";
	size_t size = 0;
	enum BaseType { cNum, cPtr, cStruct, cEnum, cVoid }
	base_type = cVoid;

protected:
	YType();
	YType(std::string name, size_t size, BaseType baseType);

	static void add(YType* type);
	static void add(TypeList& vec_types);

	friend void doTests();
	static void init();//call to sub-classes' init()
	static void terminate();//just run with the base class to delete everything

public:
	//!@throws YTypeNotFoundException
	//!@note this function does NOT skip blanks
	static YType* parse(const char* str);

	const char* className() const override;
	void print() override;
};
