/**
 * Created by cy on 2018/5/24.
 */

#include <iostream>
#include "YNumType.h"
#include "stringUtils.h"
#include "YNumType.h"

using namespace std;

YNumType::pYNumType
	YNumType::Char = new YNumType("char", 1, true, false),
	YNumType::UChar = new YNumType("char", 1, false, false),
	YNumType::Short = new YNumType("short", 2, true, false),
	YNumType::UShort = new YNumType("short", 2, false, false),
	YNumType::Int = new YNumType("int", 4, true, false),
	YNumType::UInt = new YNumType("int", 4, false, false),
	YNumType::Long = new YNumType("long", 4, true, false),
	YNumType::ULong = new YNumType("long", 4, false, false),
	YNumType::LongLong = new YNumType("long long", 8, true, false),
	YNumType::ULongLong = new YNumType("long long", 8, false, false),
	YNumType::Float = new YNumType("float", 4, true, true),
	YNumType::Double = new YNumType("double", 8, true, true),
	YNumType::LongDouble = new YNumType("long double", 16, true, true);

const char* YNumType::className() const {
	return "YNum";
}

void YNumType::init() {
	static TypeList numTypes ={ Char,UChar,Short,UShort,Int,UInt,Long,ULong,LongLong,ULongLong,Float,Double,LongDouble };
	YType::add(numTypes);
}

YNumType::YNumType(const char* name, const size_t size, const bool bIsSigned, const bool bIsDecimal)
	: YType(name, size, cNum), bIsSigned(bIsSigned), bIsDecimal(bIsDecimal) {}

void YNumType::print() {
	cout << "[YNum]{name=\"" << name << "\"}";
}

bool YNumType::isInteger(){
	return (this!=Float) && (this!=Double) && (this!=LongDouble);
}

