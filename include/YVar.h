// Created by cy on 2018/06/22
// 
// saving and parsing a val

#pragma once
#include "Excutable.h"
#include "YVal.h"
#include "YExpression.h"
#include <string>

class YVar: public Executable {
public:
	YVar(const char* name, YVal* val = nullptr);

	//just parse strings like "char c"
	//WITHOUT ';' or '='
	static YVar* parse(const char* str);

	//call me when executing the declearation statement
	void init();

	YVal* execute() override;
	~YVar() override;
	const char* className() const override;
	void print() override;

protected:
	const std::string name;
	YVal* val = nullptr;
	Executable* initial_value = nullptr;
	// const std::string name;
	// YVal* val = nullptr;
	// YExpression* initial_expression = nullptr;

	static bool isAvailableVarName(const char* name);
};
