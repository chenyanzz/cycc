 // Created by cy on 2018/06/22
// 
// saving and parsing a val

#pragma once
#include "Excutable.h"
#include "YVal.h"
#include <string>

class YVar: public Executable {
public:
	YVar(std::string& name,YVal* val=nullptr);

	//just parse strings like "char c"
	//WITHOUT ';' or '='
	static YVar* parse(const char* str);

	YVal* execute() override;
	~YVar() override;
	const char* className() const override;
	void print() override;

protected:
	const std::string name;
	YVal* val = nullptr;

	static bool isAvailableVarName(std::string& name);
};
