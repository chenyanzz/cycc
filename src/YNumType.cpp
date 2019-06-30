/**
 * Created by cy on 2018/5/24.
 */

#include <iostream>
#include "YNumType.h"
#include "stringUtils.h"
#include "YNumType.h"
#include "common.h"

using namespace std;

const char* YNumType::str_unsigned = "unsigned ";
const char* YNumType::str_signed = "signed ";

YNumType::pYNumType
	YNumType::Char = new YNumType("char", 1, true, false),
	YNumType::UChar = new YNumType("unsigned char", 1, false, false),
	YNumType::Short = new YNumType("short", 2, true, false),
	YNumType::UShort = new YNumType("unsigned short", 2, false, false),
	YNumType::Int = new YNumType("int", 4, true, false),
	YNumType::UInt = new YNumType("unsigned int", 4, false, false),
	YNumType::Long = new YNumType("long", 4, true, false),
	YNumType::ULong = new YNumType("unsigned long", 4, false, false),
	YNumType::LongLong = new YNumType("long long", 8, true, false),
	YNumType::ULongLong = new YNumType("unsigned long long", 8, false, false),
	YNumType::Float = new YNumType("float", 4, true, true),
	YNumType::Double = new YNumType("double", 8, true, true),
	YNumType::LongDouble = new YNumType("long double", 16, true, true);

const char* YNumType::className() const {return "YNumType";}

void YNumType::init() {
	static TypeList numTypes ={ Char,UChar,Short,UShort,Int,UInt,Long,ULong,LongLong,ULongLong,Float,Double,LongDouble };
	YType::add(numTypes);
}

YNumType::YNumType(const char* name, const size_t size, const bool bIsSigned, const bool bIsDecimal)
	: YType(name, size, cNum), bIsSigned(bIsSigned), bIsDecimal(bIsDecimal){}
	
YNumType* YNumType::parse(const char* str){
	if(isFirstSubStr(str,str_signed)) str+=strlen(str_signed);
	auto iter = cyfind_if(YType::types,[=](YType* type){
		return type->base_type==cNum && type->name == str;
	});
	return iter==YType::types.end()? nullptr : dynamic_cast<YNumType*>(*iter);
}

bool YNumType::isInteger(){
	return (this!=Float) && (this!=Double) && (this!=LongDouble);
}

