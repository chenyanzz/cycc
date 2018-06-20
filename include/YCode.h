// Created by cy on 2018/06/18
// 
// A class for reading code

#pragma once

#include "Excutable.h"

class YCode :public Executable{
public:
	const char* className() const override;
	void print() override;
	YVal* execute() override;

	static YCode* parseFromFile(const char* path);
	static YCode* parseFromString(const char* str);

};
