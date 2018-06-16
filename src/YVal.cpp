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

#define CHECK_NULLPTR(param) if((param)==nullptr){setLastError(new YNullptrException(#param));}

const char* YVal::className() const {

	return "YVal";
}


void YVal::print() {
	if(pType->base_type == YType::cNum) {
		const auto pNum = (YNum*)pType;
		if(pNum->bIsDecimal) {
			cout << "(YVal){num=";
			switch(pType->size) {
			case 4:
				cout << *(float*)pData;
				break;
			case 8:
				cout << *(double*)pData;
				break;
			case 16:
				cout << *(long double*)pData;
				break;
			}
			cout << ", type=";
			pType->print();
			cout << "}";
		} else {
			cout << "(YVal){data=0x";
			switch(pType->size) {
			case 1:
				cout << hex << (short)*(unsigned char*)pData; //to show char as a number
				break;
			case 2:
				cout << hex << *(unsigned short*)pData;
				break;
			case 4:
				cout << hex << *(unsigned int*)pData;
				break;
			case 8:
				cout << hex << *(unsigned long long*)pData;
				break;
			}
			cout << ", type=";
			pType->print();
			cout << "}";
		}
	}
}


bool YVal::parse(const char* s, YVal*& pVal) {
	clearLastError();
	pVal = new YVal;

	char c = *s;
	//pass if c is a identifier
	if((c >= '0' && c <= '9') || c == '-' || c == '\'') {
		if(parseDecimal(s, pVal)) return true;
		if(parseInt(s, pVal)) return true;
	}
	delete pVal;
	pVal = nullptr;
	setLastError(new YParseFailedException("YVal", s, "not a proper literal"));
	return false;
}


bool YVal::parseInt(const char* s, YVal* pVal) {
	clearLastError();
	CHECK_NULLPTR(pVal);

	YType* p_type = nullptr;

	//for char val
	if(s[0] == '\'' && s[2] == '\'') {
		pVal->pType         = YNum::Char;
		pVal->pData         = new byte[pVal->pType->size];
		*(char*)pVal->pData = s[1];
		return true;
	}

	//for int val

	int radix             = 10;
	const size_t buf_size = 100;
	static char buf[buf_size];
	strcpy_s(buf, buf_size, s);
	char* pc_num = buf;

	bool bNegative = false;

	//for chars like '\0777' '\0x88'
	if(buf[0] == '\'' && buf[strlen(buf) - 1] == '\'') {
		pc_num++;
		buf[strlen(buf) - 1] = 0;
		p_type               = YType::Char;
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
			setLastError(new YInvalidCharException(s, len - 1, "suffix \"UU\" is illegal"));
			return false;

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

	unsigned long long num   = 0;
	for(char* org_p          = pc_num; pc_num - org_p < len; pc_num++) {
		const char hexletter = (*pc_num | (char)0x20);
		int bitval;

		if(*pc_num >= '0' && *pc_num <= '9') {
			bitval = *pc_num - '0';
		} else if(hexletter >= 'a' && hexletter <= 'f') {
			bitval = hexletter - 'a' + 10;
		} else {
			setLastError(new YInvalidCharException(s, pc_num - buf, "illegal char in a number body"));
			return false;
		}

		if(bitval >= radix) {
			setLastError(new YInvalidCharException(s, pc_num - buf, "illegal char in a number body"));
			return false;
		}

		num *= radix;
		num += bitval;
	}


	/*making the YVal*/

	if(bNegative) {
		num = ~(num & ~(1 << (sizeof(num) - 1))) + 1;
	}

	pVal->pType = p_type;
	pVal->pData = new byte[pVal->pType->size];

	switch(p_type->size) {
	case 1:
		*(unsigned char*)pVal->pData = num;
		break;
	case 2:
		*(unsigned short*)pVal->pData = num;
		break;
	case 4:
		*(unsigned int*)pVal->pData = num;
		break;
	case 8:
		*(unsigned long long*)pVal->pData = num;
		break;
	}

	return true;
}


YType* YVal::type() {
	return pType;
}

YVal* YVal::execute() { return this->clone(); }

YVal* YVal::add(YVal* v1, YVal* v2) {
	const auto longdouble_v1 = v1->castTo(YType::LongDouble);
	const auto longdouble_v2 = v2->castTo(YType::LongDouble);

	const auto longdouble1 = *longdouble_v1->data<long double>();
	const auto longdouble2 = *longdouble_v2->data<long double>();

	delete longdouble_v1;
	delete longdouble_v2;

	auto* v_result = new long double(longdouble1 + longdouble2);

	auto result = new YVal(YType::LongDouble, v_result);

	return result;
}

YVal* YVal::mul(YVal* v1, YVal* v2) {
	const auto longdouble_v1 = v1->castTo(YType::LongDouble);
	const auto longdouble_v2 = v2->castTo(YType::LongDouble);

	const auto longdouble1 = *longdouble_v1->data<long double>();
	const auto longdouble2 = *longdouble_v2->data<long double>();

	delete longdouble_v1;
	delete longdouble_v2;

	auto* v_result = new long double(longdouble1 * longdouble2);

	auto result = new YVal(YType::LongDouble, v_result);

	return result;
}

YVal::~YVal() {
	if(pData == nullptr)return;

	delete[] pData;
	pData = nullptr;
}


bool YVal::parseDecimal(const char* s, YVal* pVal) {
	clearLastError();
	CHECK_NULLPTR(pVal);

	if(!strcmp(s, ".")) {
		setLastError(new YException("[YException] \"%s\" is not a proper decimal", s));
		return false;
	}

	const size_t buf_size = 100;
	static char buf[buf_size];
	strcpy_s(buf, buf_size, s);
	char* pc_num = buf;
	size_t len   = min(strlen(s), buf_size);

	bool bNegative = false;
	char* pc_dot   = pc_num;

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

		setLastError(new YInvalidCharException(s, len - 1, "the suffix is illegal for decimal"));
		return false;
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
			setLastError(new YException("[YException] \"%s\" is not a proper decimal", buf));
			return false;
		}
		pc_dot++;
	}

	YVal buf_val;

	unsigned long long int_val = 0;

	static char buf2[100];
	memcpy(buf2, pc_num, pc_dot - pc_num);
	buf2[pc_dot - pc_num] = 0;
	strcat(buf2, "ll");

	if(pc_dot != pc_num) {
		if(!parseInt(buf2, &buf_val)) return false;
		int_val = *(unsigned long long*)buf_val.pData;
	}

	/*decimal part*/

	long double dec_val = 0;

	for(char* pc = buf + len - 1; *pc != '.'; pc--) {
		if(*pc >= '0' && *pc <= '9') {
			dec_val += *pc - '0';
			dec_val /= 10;
		} else {
			setLastError(new YInvalidCharException(s, pc - buf, "illegal char in a number body"));
			return false;
		}
	}

	long double val = dec_val + int_val;

	pVal->pType = ptype;
	pVal->pData = new byte[pVal->pType->size];

	if(bNegative)val = -val;

	switch(ptype->size) {
	case 4:
		*(float*)pVal->pData = val;
		break;
	case 8:
		*(double*)pVal->pData = val;
		break;
	case 16:
		*(long double*)pVal->pData = val;
		break;
	}

	return true;
}

YVal* YVal::castTo(YType* pNewType) {
	YType* p_old_type = pType;
	byte* p_old_data  = pData;

	//the cases without a memory-change:
	//anyone is not a number
	//or both are int's
	if(
		pNewType->base_type != YType::cNum ||
		p_old_type->base_type != YType::cNum ||
		(!((YNum*)p_old_type)->bIsDecimal && !((YNum*)pNewType)->bIsDecimal)
	) {
		return new YVal(pNewType, p_old_data);
	}

	//deal with the cast between int and decimal
	YVal* pNewVal  = new YVal;
	pNewVal->pType = pNewType;

	if(pNewType->base_type == YType::cNum) {
		YNum* pOldNum = (YNum*)p_old_type;
		YNum* pNewNum = (YNum*)pNewType;

		if(pOldNum->bIsDecimal && pNewNum->bIsDecimal) {
			//from int to dec
			long double oldval = 0.0l;
			switch(p_old_type->size) {
			case 4:
				oldval = *(float*)p_old_data;
				break;
			case 8:
				oldval = *(double*)p_old_data;
				break;
			case 16:
				oldval = *(long double*)p_old_data;
				break;
			}

			void* p = nullptr;
			switch(pNewType->size) {
			case 4: {
				p          = (float*)new byte[YType::Float->size];
				*(float*)p = oldval;
				break;
			}
			case 8: {
				p           = (double*)new byte[YType::Double->size];
				*(double*)p = oldval;
				break;
			}
			case 16: {
				p                = (long double*)new byte[YType::LongDouble->size];
				*(long double*)p = oldval;
				break;
			}

			}
			pNewVal->pData = (byte*)p;
		} else if(pOldNum->bIsDecimal) {
			//from dec to int
			long long* pll = (long long*)new byte[YType::LongLong->size];
			switch(p_old_type->size) {
			case 4:
				*pll = *(float*)p_old_data;
				break;
			case 8:
				*pll = *(double*)p_old_data;
				break;
			case 16:
				*pll = *(long double*)p_old_data;
				break;
			}
			pNewVal->pData = (byte*)pll;
		} else {
			//from int to dec
			long long oldval = 0;
			switch(p_old_type->size) {
			case 1:
				oldval = *(unsigned char*)p_old_data;
				break;
			case 2:
				oldval = *(unsigned short*)p_old_data;
				break;
			case 4:
				oldval = *(unsigned int*)p_old_data;
				break;
			case 8:
				oldval = *(unsigned long long*)p_old_data;
				break;
			}

			void* p = nullptr;
			switch(pNewType->size) {
			case 4: {
				p          = (float*)new byte[YType::Float->size];
				*(float*)p = oldval;
				break;
			}
			case 8: {
				p           = (double*)new byte[YType::Double->size];
				*(double*)p = oldval;
				break;
			}
			case 16: {
				p                = (long double*)new byte[YType::LongDouble->size];
				*(long double*)p = oldval;
				break;
			}

			}
			pNewVal->pData = (byte*)p;
		}
	}

	return pNewVal;

}


YVal* YVal::clone() {
	return new YVal(this);
}


YVal::YVal(YType* ptype, void* pdata) {
	this->pType = ptype;
	this->pData = new byte[this->pType->size];

	memset(this->pData, 0, this->pType->size);
	memcpy(this->pData, pdata, min(this->pType->size, ptype->size));
}

YVal::YVal(YVal* pVal) : YVal(pVal->pType, pVal->pData) {}

template <typename Type>
Type*& YVal::data() {
	return (Type*&)pData;
}
