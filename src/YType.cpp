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

const char* YType::str_unsigned = "unsigned ";
const char* YType::str_signed = "signed ";

YType::YType() : YType("$UNINITIALIZED", 0, cVoid) {}

YType::YType(string name, size_t size, BaseType baseType)
	: name(name), size(size), base_type(baseType) {}

void YType::add(YType* type) { types.insert(type); }
void YType::add(TypeList& vec_types) { types.insert(vec_types.begin(), vec_types.end()); }

YType* YType::parse(const char* str) {
	string str_type = str;

	const bool isSigned = isFirstSubStr(str, str_signed);
	if(isSigned) {
		str_type = string(str + strlen(str_signed));
	}

	const auto it = cyfind_if(types, [=](YType* pt) -> bool {return pt->name == str_type; });
	if(it == types.end()) {
		throw YTypeNotFoundException(str);
	}

	if(isSigned) {
		if((*it)->base_type != cNum) {
			throw YTypeNotFoundException(str);
		}
	}


	return *it;
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
