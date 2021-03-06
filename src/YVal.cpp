/**
 * Created by cy on 2018/5/26.
 */

#include <iostream>
#include <cstring>
#include "YNumType.h"
#include "YVal.h"
#include "YException.h"
#include "stringUtils.h"
#include "numberUtils.h"

using namespace std;

#pragma warning(disable:4244) //casting

#define CHECK_NULLPTR(param) if((param)==nullptr){throw YNullptrException(#param);}

template <typename func_2op_t>
YVal* YVal::exec_op2(YVal* v1, YVal* v2, func_2op_t operation) {
	YVal* pVal_longdouble1 = v1->castTo(YNumType::LongDouble);
	YVal* pVal_longdouble2 = v2->castTo(YNumType::LongDouble);
	const long double number1 = pVal_longdouble1->data<long double>();
	const long double number2 = pVal_longdouble2->data<long double>();
	delete pVal_longdouble1, pVal_longdouble2;

	return YVal::newFrom(YNumType::LongDouble, operation(number1, number2));
}

template <typename func_1op_t>
YVal* YVal::exec_op1(YVal* v, func_1op_t operation) {
	YVal* pVal_longdouble = v->castTo(YNumType::LongDouble);
	const long double number = pVal_longdouble->data<long double>();
	delete pVal_longdouble;

	return YVal::newFrom(YNumType::LongDouble, operation(number));
}

long double YVal::getNum(YType* pType) {
	long double num = 0;
	if(pType == YNumType::Float)num = this->data<float>();
	else if(pType == YNumType::Double)num = this->data<double>();
	else if(pType == YNumType::LongDouble)num = this->data<long double>();
	else if(pType == YNumType::Char)num = (int)this->data<char>(); /*do not show as a char*/
	else if(pType == YNumType::Short)num = this->data<short>();
	else if(pType == YNumType::Int)num = this->data<int>();
	else if(pType == YNumType::Long)num = this->data<long>();
	else if(pType == YNumType::LongLong)num = this->data<long long>();
	else if(pType == YNumType::UChar)num = (int)this->data<unsigned char>(); /*do not show as a char*/
	else if(pType == YNumType::UShort)num = this->data<unsigned short>();
	else if(pType == YNumType::UInt)num = this->data<unsigned int>();
	else if(pType == YNumType::ULong)num = this->data<unsigned long>();
	else if(pType == YNumType::ULongLong)num = this->data<unsigned long long>();
	return num;
}

template <typename T>
void YVal::setNum(T equal_what) {
	if(this->pType == YNumType::Float)this->data<float>() = equal_what;
	else if(this->pType == YNumType::Double)this->data<double>() = equal_what;
	else if(this->pType == YNumType::LongDouble)this->data<long double>() = equal_what;
	else if(this->pType == YNumType::Char)this->data<char>() = equal_what;
	else if(this->pType == YNumType::Short)this->data<short>() = equal_what;
	else if(this->pType == YNumType::Int)this->data<int>() = equal_what;
	else if(this->pType == YNumType::Long)this->data<long>() = equal_what;
	else if(this->pType == YNumType::LongLong)this->data<long long>() = equal_what;
	else if(this->pType == YNumType::UChar)this->data<unsigned char>() = equal_what;
	else if(this->pType == YNumType::UShort)this->data<unsigned short>() = equal_what;
	else if(this->pType == YNumType::UInt)this->data<unsigned int>() = equal_what;
	else if(this->pType == YNumType::ULong)this->data<unsigned long>() = equal_what;
	else if(this->pType == YNumType::ULongLong)this->data<unsigned long long>() = equal_what;
}

const char* YVal::className() const { return "YVal"; }

void YVal::print() {
	if(pType->base_type == YNumType::cNum) {
		cout << getNum(pType);
	}
}

YVal* YVal::parse(const char* s) {
	if(s[0] == '+') s++;

	try { return parseDecimal(s); } catch(YException&) {}
	try { return parseInt(s); } catch(YException&) {}

	throw YParseFailedException("YVal", s, "not a proper literal");
}

YVal* YVal::parseInt(const char* s) {
	YType* p_type = nullptr;

	//for char val				
	if(s[0] == '\'' && s[2] == '\'') {
		return YVal::newFrom(YNumType::Char, s[1]);
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
		p_type = YNumType::Char;
	}

	/*prefix*/
	switch(*pc_num) {
	case '-':
		bNegative = true;
	case '+':
		pc_num++;
		const char c = *pc_num;
		if(!isHexDigit(c)) throw YParseFailedException("YVal", s, "is not a number");
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
			p_type = YNumType::ULongLong;
		} else if(isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2)) {
			len -= 2;
			p_type = YNumType::ULong;
		} else if(isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2)) {
			len -= 2;
			p_type = YNumType::LongLong;
		} else if(isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {
			throw YInvalidCharException(s, len - 1, "suffix \"UU\" is illegal");
		} else if(isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
			len--;
			p_type = YNumType::Long;
		} else if(isLastSubStr(pc_num, len, "U", 1) || isLastSubStr(pc_num, len, "u", 1)) {
			len--;
			p_type = YNumType::UInt;
		} else {
			p_type = YNumType::Int;
		}
	}

	/*number part*/

	unsigned long long num = 0;
	for(char* org_p = pc_num; pc_num - org_p < len; pc_num++) {
		const char hexletter = (*pc_num | (char)0x20);
		int bitval;

		if(isDecDigit(*pc_num)) {
			bitval = *pc_num - '0';
		} else if(hexletter >= 'a' && hexletter <= 'f') {
			bitval = hexletter - 'a' + 10;
		} else {
			throw YInvalidCharException(s, pc_num - buf, "illegal char in a number body");
		}

		if(bitval >= radix) {
			throw YInvalidCharException(s, pc_num - buf, "illegal char in a number body");
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
YVal* YVal::add(YVal* v1, YVal* v2) { return exec_op2(v1, v2, [](auto v1, auto v2) { return v1 + v2; }); }
YVal* YVal::sub(YVal* v1, YVal* v2) { return exec_op2(v1, v2, [](auto v1, auto v2) { return v1 - v2; }); }
YVal* YVal::mul(YVal* v1, YVal* v2) { return exec_op2(v1, v2, [](auto v1, auto v2) { return v1 * v2; }); }
YVal* YVal::div(YVal* v1, YVal* v2) {
	YType* t1 = v1->type();
	YType* t2 = v2->type();

	//if 1/0 throw an error
	if( (t1->base_type==YType::cNum) && dynamic_cast<YNumType*>(t1)->isInteger() &&
		(t2->base_type==YType::cNum) && dynamic_cast<YNumType*>(t2)->isInteger() &&
		v2->castTo(YNumType::LongDouble)->data<long double>()==0 ) {
		throw YDividedByZeroException();
	}
	return exec_op2(v1, v2, [](auto v1, auto v2) { return v1 / v2; });
}
YVal* YVal::neg(YVal* v) { return exec_op1(v, [](auto v) { return -v; }); }

YVal* YVal::parseDecimal(const char* s) {

	//no dot or only a dot'.'
	if(!strchr(s,'.') || !strcmp(s, ".")) {
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

	YType* ptype = YNumType::Double;

	if(isLastSubStr(pc_num, len, "F", 1) || isLastSubStr(pc_num, len, "f", 1)) {
		len--;
		ptype = YNumType::Float;
	} else if(isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
		len--;
		ptype = YNumType::LongDouble;
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
		if(isDecDigit(*pc)) {
			dec_val += *pc - '0';
			dec_val /= 10;
		} else {
			throw YInvalidCharException(s, pc - buf, "illegal char in a number body");
		}
	}

	long double val = dec_val + int_val;

	YVal* pVal = new YVal(ptype);

	if(bNegative)val = -val;

	if(ptype == YNumType::Float) pVal->data<float>() = val;
	else if(ptype == YNumType::Double) pVal->data<double>() = val;
	else if(ptype == YNumType::LongDouble) pVal->data<long double>() = val;

	return pVal;
}

YVal* YVal::castTo(YType* pNewType) {
	auto pOldType = pType;
	byte* pOldData = pData;

	//the cases without a memory-change:
	//anyone is not a number
	//or both are int's
	if(
		pNewType->base_type != YNumType::cNum ||
		pOldType->base_type != YNumType::cNum ||
		(!((YNumType*)pOldType)->bIsDecimal && !((YNumType*)pNewType)->bIsDecimal)
	) {
		return new YVal(pNewType, pOldData);
	}

	//deal with the cast between int and decimal
	YVal* pNewVal = new YVal(pNewType);

	if(pNewType->base_type == YNumType::cNum) {

		long double oldval = 0.0l;

		oldval = getNum(pType);
		pNewVal->setNum(oldval);
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
	memcpy(this->pData, pdata, this->pType->size);
}

YVal::YVal(YVal* pVal): YVal(pVal->pType, pVal->pData) {}

template <typename T>
YVal* YVal::newFrom(YType* type, T data) { return new YVal(type, &data); }

YVal::~YVal() {
	deleteData();
}

void YVal::deleteData() {
	delete pData;
	pData = nullptr;
	pType = nullptr;
}

template <typename Type>
Type& YVal::data() { return * reinterpret_cast<Type*>(pData); }
