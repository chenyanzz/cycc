/**
 * Created by cy on 2018/5/24.
 */

#include <iostream>
#include <cstring>
#include "YException.h"
#include "stringUtils.h"
#include "YType.h"
#include "YNum.h"
#include "common.h"

using namespace std;


set<YType*> YType::types;

const char* YType::str_unsigned = "unsigned ";

const char* YType::str_signed = "signed ";

YType::PType
YType::Char = nullptr,
YType::UChar = nullptr,
YType::Short = nullptr,
YType::UShort = nullptr,
YType::Int = nullptr,
YType::UInt = nullptr,
YType::Long = nullptr,
YType::ULong = nullptr,
YType::LongLong = nullptr,
YType::ULongLong = nullptr,
YType::Float = nullptr,
YType::Double = nullptr,
YType::LongDouble = nullptr;

bool YType::add(const char* name, const int size, const BaseType baseType, YType*& pType) {
	if (baseType == cNum) {
		YNum* pNum = new YNum;
		pNum->bIsSigned = true;
		pNum->name = name;

		if (isFirstSubStr(name, str_unsigned)) {
			pNum->bIsSigned = false;
		}

		//NOTE that "signed int" and "int" is same a type
		if (isFirstSubStr(name, str_signed)) {
			pNum->name = pNum->name.c_str() + strlen(str_signed);
		}

		if (!(strcmp(name, "float") && strcmp(name, "double") && strcmp(name, "long double"))) {
			pNum->bIsDecimal = true;
		}

		pType = (YType*)pNum;
	}

	pType->size = size;
	pType->base_type = baseType;

	types.insert(pType);
	return true;
}


YType* YType::parse(const char* code) {

	string str_type = code;
	const bool isSigned = isFirstSubStr(code, str_signed);
	if (isSigned) {
		str_type = string(code + strlen(str_signed));
	}

	const auto it = cyfind_if(types, [=](YType* pt) -> bool {return pt->name == str_type; });
	if (it == types.end()) {
		throw new YTypeNotFoundException(code);
	}

	if (isSigned) {
		if ((*it)->base_type != cNum) {
			throw new YTypeNotFoundException(code);
		}
	}

	return *it;
}


void YType::init() {
	YType::add("long long", 8, YType::cNum, YType::LongLong);
	YType::add("unsigned long long", 8, YType::cNum, ULongLong);

	YType::add("long", 4, YType::cNum, Long);
	YType::add("unsigned long", 4, YType::cNum, ULong);

	YType::add("int", 4, YType::cNum, Int);
	YType::add("unsigned int", 4, YType::cNum, UInt);

	YType::add("short", 2, YType::cNum, Short);
	YType::add("unsined short", 2, YType::cNum, UShort);

	YType::add("char", 1, YType::cNum, Char);
	YType::add("unsigned char", 1, YType::cNum, UChar);

	YType::add("float", 4, YType::cNum, Float);
	YType::add("double", 8, YType::cNum, Double);
	YType::add("long double", 16, YType::cNum, LongDouble);
}


void YType::terminate() {
	for (auto p:types) {
		delete p;
	}
	types.clear();
}


void YType::print() {
	cout << "[" << className() << "]{name=\"" << name << "\", size=" << size << ", base_type=" << base_type << "}";
}


const char* YType::className() const {
	return "YType";
}