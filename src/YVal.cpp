/**
 * Created by cy on 2018/5/26.
 */

#include <iostream>
#include <cstring>
#include "YNum.h"
#include "YVal.h"
#include "YException.h"
#include "stringUtils.h"

using namespace std;

#define CHECK_NULLPTR(param) if((param)==nullptr){throw(new YNullptrException(#param));}

//stat: do with (long double) v1, v2
#define implement_operation2(stat)	{	\
const auto longdouble_v1 = v1->castTo(YType::LongDouble);	\
const auto longdouble_v2 = v2->castTo(YType::LongDouble);	\
const auto v1 = longdouble_v1->data<long double>();	\
const auto v2 = longdouble_v2->data<long double>();	\
delete longdouble_v1;	\
delete longdouble_v2;	\
auto* v_result = new long double(stat);	\
auto result = new YVal(YType::LongDouble, v_result);	\
delete v_result;	\
return result;	\
}

//stat: do with (long double) v
#define implement_operation1(stat)	{	\
const auto longdouble_v = v->castTo(YType::LongDouble);	\
const auto v = longdouble_v->data<long double>();	\
delete longdouble_v;	\
auto* v_result = new long double(stat);	\
auto result = new YVal(YType::LongDouble, v_result);	\
delete v_result;	\
return result;	\
}

//stat: do with (long double)num = pVal->data<pType>()
#define getNum(pType,pVal,stat){	\
long double num=0;	\
if (pType == YType::Float)num = pVal->data<float>();	\
else if (pType == YType::Double)num = pVal->data<double>();	\
else if (pType == YType::LongDouble)num = pVal->data<long double>();	\
else if (pType == YType::Char)num = (int)pVal->data<char>();/*do not show as a char*/	\
else if (pType == YType::Short)num = pVal->data<short>();	\
else if (pType == YType::Int)num = pVal->data<int>();	\
else if (pType == YType::Long)num = pVal->data<long>();	\
else if (pType == YType::LongLong)num = pVal->data<long long>();	\
else if (pType == YType::UChar)num = (int)pVal->data<unsigned char>();/*do not show as a char*/	\
else if (pType == YType::UShort)num = pVal->data<unsigned short>();	\
else if (pType == YType::UInt)num = pVal->data<unsigned int>();	\
else if (pType == YType::ULong)num = pVal->data<unsigned long>();	\
else if (pType == YType::ULongLong)num = pVal->data<unsigned long long>();	\
stat;	\
}


#define setNum(pVal,equal_what){	\
if (pVal->pType == YType::Float)pVal->data<float>() = equal_what;	\
else if (pVal->pType == YType::Double)pVal->data<double>() = equal_what;	\
else if (pVal->pType == YType::LongDouble)pVal->data<long double>() = equal_what;	\
else if (pVal->pType == YType::Char)pVal->data<char>() = equal_what;	\
else if (pVal->pType == YType::Short)pVal->data<short>() = equal_what;	\
else if (pVal->pType == YType::Int)pVal->data<int>() = equal_what;	\
else if (pVal->pType == YType::Long)pVal->data<long>() = equal_what;	\
else if (pVal->pType == YType::LongLong)pVal->data<long long>() = equal_what;	\
else if (pVal->pType == YType::UChar)pVal->data<unsigned char>() = equal_what;	\
else if (pVal->pType == YType::UShort)pVal->data<unsigned short>() = equal_what;	\
else if (pVal->pType == YType::UInt)pVal->data<unsigned int>() = equal_what;	\
else if (pVal->pType == YType::ULong)pVal->data<unsigned long>() = equal_what;	\
else if (pVal->pType == YType::ULongLong)pVal->data<unsigned long long>() = equal_what;	\
}

const char* YVal::className() const {
	return "YVal";
}

void YVal::print() {
	if(pType->base_type == YType::cNum) {
		getNum(pType, this, cout << num);
	}
}

YVal* YVal::parse(const char* s) {
	if(s[0] == '+') s++;

	try { return parseDecimal(s); } catch(YException& e) {}
	try { return parseInt(s); } catch(YException& e) {}

	throw YParseFailedException("YVal", s, "not a proper literal");
}

YVal* YVal::parseInt(const char* s) {
	YType* p_type = nullptr;

	//for char val
	if(s[0] == '\'' && s[2] == '\'') {
		YVal* pVal = new YVal(YNum::Char);
		pVal->data<char>() = s[1];
		return pVal;
	}

	//for int val

	int radix = 10;
	const size_t buf_size = 100;
	static char buf[buf_size];
	strcpy_s(buf, buf_size, s);
	char* pc_num = buf;

	bool bNegative = false;

	//for chars like '\0777' '\0x88'
	if(buf[0] == '\'' && buf[strlen(buf) - 1] == '\'') {
		pc_num++;
		buf[strlen(buf) - 1] = 0;
		p_type = YType::Char;
	}

	/*prefix*/

	switch(*pc_num) {
	case '-':
		bNegative = true;
	case '+':
		pc_num++;
	}

	//deal with hex oct bin
	if(*pc_num == '0') {
		pc_num++;
		switch(*pc_num) {
		case 'X':
		case 'x':
			radix = 16;
			pc_num++;
			break;

		case 'B':
		case 'b':
			radix = 2;
			pc_num++;
			break;

		default:
			radix = 8;
			break;
		}
	}

	/*suffix*/
	size_t len = min(strlen(pc_num), buf_size - (pc_num - buf));

	//maybe ptype is set before
	if(p_type == nullptr) {
		if(isLastSubStr(pc_num, len, "ULL", 3) || isLastSubStr(pc_num, len, "ull", 3) ||
			isLastSubStr(pc_num, len, "LLU", 3) || isLastSubStr(pc_num, len, "llu", 3)) {
			len -= 3;
			p_type = YType::ULongLong;
		} else if(isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2)) {
			len -= 2;
			p_type = YType::ULong;
		} else if(isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2)) {
			len -= 2;
			p_type = YType::LongLong;
		} else if(isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {
			throw YInvalidCharException(s, len - 1, "suffix \"UU\" is illegal");
		} else if(isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
			len--;
			p_type = YType::Long;
		} else if(isLastSubStr(pc_num, len, "U", 1) || isLastSubStr(pc_num, len, "u", 1)) {
			len--;
			p_type = YType::UInt;
		} else {
			p_type = YType::Int;
		}
	}

	/*number part*/

	unsigned long long num = 0;
	for(char* org_p = pc_num; pc_num - org_p < len; pc_num++) {
		const char hexletter = (*pc_num | (char)0x20);
		int bitval;

		if(*pc_num >= '0' && *pc_num <= '9') {
			bitval = *pc_num - '0';
		} else if(hexletter >= 'a' && hexletter <= 'f') {
			bitval = hexletter - 'a' + 10;
		} else {
			throw(new YInvalidCharException(s, pc_num - buf, "illegal char in a number body"));
		}

		if(bitval >= radix) {
			throw(new YInvalidCharException(s, pc_num - buf, "illegal char in a number body"));
		}

		num *= radix;
		num += bitval;
	}


	/*making the YVal*/

	if(bNegative) {
		num = ~(num & ~(1 << (sizeof(num) - 1))) + 1;
	}

	YVal* pVal = new YVal(p_type);

	switch(p_type->size) {
	case 1:
		pVal->data<unsigned char>() = num;
		break;
	case 2:
		pVal->data<unsigned short>() = num;
		break;
	case 4:
		pVal->data<unsigned int>() = num;
		break;
	case 8:
		pVal->data<unsigned long long>() = num;
		break;
	}

	return pVal;
}

YType* YVal::type() { return pType; }

YVal* YVal::execute() { return this->clone(); }

YVal* YVal::add(YVal* v1, YVal* v2) {
	implement_operation2(v1 + v2);
}

YVal* YVal::sub(YVal* v1, YVal* v2) {
	implement_operation2(v1 - v2);
}

YVal* YVal::mul(YVal* v1, YVal* v2) {
	implement_operation2(v1 * v2);
}

YVal* YVal::div(YVal* v1, YVal* v2) {
	implement_operation2(v1 / v2);
}

YVal* YVal::neg(YVal* v) {
	implement_operation1(-v);
}

YVal* YVal::parseDecimal(const char* s) {

	if(!strcmp(s, ".")) {
		throw YException("[YException] \"%s\" is not a proper decimal", s);
	}

	const size_t buf_size = 100;
	static char buf[buf_size];
	strcpy_s(buf, buf_size, s);
	char* pc_num = buf;
	size_t len = min(strlen(s), buf_size);

	bool bNegative = false;
	char* pc_dot = pc_num;

	/*prefix*/

	switch(*pc_num) {
	case '-':
		bNegative = true;
	case '+':
		pc_num++;
	}

	/*suffix*/
	if(isLastSubStr(pc_num, len, "ULL", 3) || isLastSubStr(pc_num, len, "ull", 3) ||
		isLastSubStr(pc_num, len, "LLU", 3) || isLastSubStr(pc_num, len, "llu", 3) ||
		isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2) ||
		isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2) ||
		isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {

		throw YInvalidCharException(s, len - 1, "the suffix is illegal for decimal");
	}

	YType* ptype = YType::Double;

	if(isLastSubStr(pc_num, len, "F", 1) || isLastSubStr(pc_num, len, "f", 1)) {
		len--;
		ptype = YType::Float;
	} else if(isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
		len--;
		ptype = YType::LongDouble;
	}

	/*int part*/

	while(*pc_dot != '.') {
		if(*pc_dot == 0) {
			throw YException("[YException] \"%s\" is not a proper decimal", buf);
		}
		pc_dot++;
	}

	unsigned long long int_val = 0;

	static char buf2[100];
	memcpy(buf2, pc_num, pc_dot - pc_num);
	buf2[pc_dot - pc_num] = 0;
	strcat(buf2, "ll");

	if(pc_dot != pc_num) {
		YVal* buf_val = parseInt(buf2);
		int_val = buf_val->data<unsigned long long>();
		delete buf_val;
	}

	/*decimal part*/

	long double dec_val = 0;

	for(char* pc = buf + len - 1; *pc != '.'; pc--) {
		if(*pc >= '0' && *pc <= '9') {
			dec_val += *pc - '0';
			dec_val /= 10;
		} else {
			throw YInvalidCharException(s, pc - buf, "illegal char in a number body");
		}
	}

	long double val = dec_val + int_val;

	YVal* pVal = new YVal(ptype);

	if(bNegative)val = -val;

	if(ptype == YType::Float) pVal->data<float>() = val;
	else if(ptype == YType::Double) pVal->data<double>() = val;
	else if(ptype == YType::LongDouble) pVal->data<long double>() = val;

	return pVal;
}

YVal* YVal::castTo(YType* pNewType) {
	auto pOldType = pType;
	byte* pOldData = pData;

	//the cases without a memory-change:
	//anyone is not a number
	//or both are int's
	if(
		pNewType->base_type != YType::cNum ||
		pOldType->base_type != YType::cNum ||
		(!((YNum*)pOldType)->bIsDecimal && !((YNum*)pNewType)->bIsDecimal)
	) {
		return new YVal(pNewType, pOldData);
	}

	//deal with the cast between int and decimal
	YVal* pNewVal = new YVal(pNewType);

	if(pNewType->base_type == YType::cNum) {

		long double oldval = 0.0l;

		getNum(pType, this, oldval = num);
		setNum(pNewVal, oldval);
	}

	return pNewVal;

}

YVal* YVal::clone() { return new YVal(this); }

YVal::YVal(YType* ptype) {
	this->pType = ptype;
	this->pData = new byte[this->pType->size];
}

YVal::YVal(YType* ptype, void* pdata): YVal(ptype) {
	memset(this->pData, 0, this->pType->size);
	memcpy(this->pData, pdata, min(this->pType->size, ptype->size));
}

YVal::YVal(YVal* pVal) : YVal(pVal->pType, pVal->pData) {}

YVal::~YVal() {
	delete pData;
	pData = nullptr;
	pType = nullptr;
}

template <typename Type>
Type& YVal::data() { return *(Type*)pData; }
