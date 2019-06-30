/**
 * Created by cy on 2018/5/24.
 */

#include <iostream>
#include <cstring>
#include "YException.h"
#include "stringUtils.h"
#include "YType.h"
#include "YNumType.h"
#include "common.h"
#include <functional>

using namespace std;

YType::TypeList YType::types = TypeList();

YType::YType() : YType("$UNINITIALIZED", 0, cVoid) {}

YType::YType(string name, size_t size, BaseType baseType)
	: name(name), size(size), base_type(baseType) {}

void YType::add(YType* type) { types.insert(type); }
void YType::add(TypeList& vec_types) { types.insert(vec_types.begin(), vec_types.end()); }

YType* YType::parse(const char* str) {
	YType* type = YNumType::parse(str);// ||someclazz::parse()||...
	if(!type) throw YTypeNotFoundException(str);
	return type;
}

void YType::init() {
	YNumType::init();
}

void YType::terminate() {
	for(auto p : types) {
		delete p;
	}
	types.clear();
}

void YType::print() {
	cout << "[" << className() << "]{name=\"" << name << "\", size=" << size << ", base_type=" << base_type << "}";
}

const char* YType::className() const { return "YType"; }