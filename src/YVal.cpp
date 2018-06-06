/**
 * Created by cy on 2018/5/26.
 */

#include <iostream>
#include <cmath>
#include <cstring>
#include "YNum.h"
#include "YVal.h"
#include "YException.h"
#include "stringUtils.h"

using namespace std;

#define CHECK_NULLPTR(param) if(param==nullptr){setLastError(new YNullptrException(#param));}

const char* YVal::className() const {
	return "YVal";
}


void YVal::print() {
	if (pType->baseType == YType::cNum) {
		YNum* pNum = (YNum*)pType;
		if (pNum->bIsDecimal) {
			cout << "(YVal){num=";
			switch (pType->size) {
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
		}
		else {
			cout << "(YVal){data=0x";
			switch (pType->size) {
			case 1:
				cout << hex << *(unsigned char*)pData;
				break;
			case 2:
				cout << hex << *(unsigned short*)pData;
				break;
			case 4:
				cout << hex << *(unsigned int*)pData;
				break;
			case 8:
				cout << hex << *(unsigned long long*) pData;
				break;
			}
			cout << ", type=";
			pType->print();
			cout << "}";
		}
	}
}


bool YVal::parse(char* s, YVal*& pVal) {
	clearLastError();
	pVal = new YVal;

	if (parseDecimal(s, pVal)) return true;
	if (parseInt(s, pVal)) return true;


	delete pVal;
	setLastError(new YParseFailedException("YVal", s, "not a proper literal"));
	return false;
}


bool YVal::parseInt(const char* __s, YVal* pVal) {
	clearLastError();
	CHECK_NULLPTR(pVal);

	YType* ptype = nullptr;
	void* pdata = nullptr;

	//for char val
	if (__s[0] == '\'' && __s[2] == '\'') {
		pVal->pType = YNum::Char;
		pVal->pData = new byte[pVal->pType->size];
		*(char*)pVal->pData = __s[1];
		return true;
	}

	//for int val

	int radix = 10;
	const size_t buf_size = 100;
	static char buf[buf_size];
	strcpy_s(buf, buf_size, __s);
	char* pc_num = buf;

	bool bNegative = false;

	//for chars like '\0777' '\0x88'
	if (buf[0] == '\'' && buf[strlen(buf) - 1] == '\'') {
		pc_num++;
		buf[strlen(buf) - 1] = 0;
		ptype = YType::Char;
	}

	/*prefix*/

	switch (*pc_num) {
	case '-':
		bNegative = true;
	case '+':
		pc_num++;
	}

	//deal with hex oct bin
	if (*pc_num == '0') {
		pc_num++;
		switch (*pc_num) {
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

		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
			radix = 8;
			break;
		}
	}

	/*suffix*/
	size_t len = min(strlen(pc_num), buf_size - (pc_num - buf));

	//maybe ptype is set before
	if (ptype == nullptr) {
		if (isLastSubStr(pc_num, len, "ULL", 3) || isLastSubStr(pc_num, len, "ull", 3) ||
			isLastSubStr(pc_num, len, "LLU", 3) || isLastSubStr(pc_num, len, "llu", 3)) {
			len -= 3;
			ptype = YType::ULongLong;
		}
		else if (isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2)) {
			len -= 2;
			ptype = YType::ULong;
		}
		else if (isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2)) {
			len -= 2;
			ptype = YType::LongLong;
		}
		else if (isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {
			setLastError(new YInvalidCharException(__s, len - 1, "suffix \"UU\" is illegal"));
			return false;

		}
		else if (isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
			len--;
			ptype = YType::Long;
		}
		else if (isLastSubStr(pc_num, len, "U", 1) || isLastSubStr(pc_num, len, "u", 1)) {
			len--;
			ptype = YType::UInt;
		}
		else {
			ptype = YType::Int;
		}
	}

	/*number part*/

	unsigned long long num = 0;
	for (char* org_p = pc_num; pc_num - org_p < len; pc_num++) {
		char hexletter = (*pc_num | (char)0x20);
		int bitval;

		if (*pc_num >= '0' && *pc_num <= '9') {
			bitval = *pc_num - '0';
		}
		else if (hexletter >= 'a' && hexletter <= 'f') {
			bitval = hexletter - 'a' + 10;
		}
		else {
			setLastError(new YInvalidCharException(__s, pc_num - buf, "illegal char in a number body"));
			return false;
		}

		if (bitval >= radix) {
			setLastError(new YInvalidCharException(__s, pc_num - buf, "illegal char in a number body"));
			return false;
		}

		num *= radix;
		num += bitval;
	}


	/*making the YVal*/

	if (bNegative) {
		num = ~(num & ~(1 << (sizeof(num) - 1))) + 1;
	}

	pVal->pType = ptype;
	pVal->pData = new byte[pVal->pType->size];

	switch (ptype->size) {
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
		*(unsigned long long*) pVal->pData = num;
		break;
	}

	return true;
}


YVal::~YVal() {
	if (pData == nullptr)return;

	delete[] pData;
	pData = nullptr;
}


bool YVal::parseDecimal(const char* __s, YVal* pVal) {
	clearLastError();
	CHECK_NULLPTR(pVal);

	if (!strcmp(__s, ".")) {
		setLastError(new YException("[YException] \"%s\" is not a proper decimal", __s));
		return false;
	}

	const size_t buf_size = 100;
	static char buf[buf_size];
	strcpy_s(buf, buf_size, __s);
	char* pc_num = buf;
	size_t len = min(strlen(__s), buf_size);

	bool bNegative = false;
	char* pc_dot = pc_num;

	/*prefix*/

	switch (*pc_num) {
	case '-':
		bNegative = true;
	case '+':
		pc_num++;
	}

	/*suffix*/
	YType* ptype = nullptr;
	if (isLastSubStr(pc_num, len, "ULL", 3) || isLastSubStr(pc_num, len, "ull", 3) ||
		isLastSubStr(pc_num, len, "LLU", 3) || isLastSubStr(pc_num, len, "llu", 3) ||
		isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2) ||
		isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2) ||
		isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {

		setLastError(new YInvalidCharException(__s, len - 1, "the suffix is illegal for decimal"));
		return false;
	}

	ptype = YType::Double;

	if (isLastSubStr(pc_num, len, "F", 1) || isLastSubStr(pc_num, len, "f", 1)) {
		len--;
		ptype = YType::Float;
	}
	else if (isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
		len--;
		ptype = YType::LongDouble;
	}

	/*int part*/

	while (*pc_dot != '.') {
		if (*pc_dot == 0) {
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

	if (pc_dot != pc_num) {
		if (!parseInt(buf2, &buf_val)) return false;
		int_val = *(unsigned long long*)buf_val.pData;
	}

	/*decimal part*/

	long double dec_val = 0;

	for (char* pc = buf + len - 1; *pc != '.'; pc--) {
		if (*pc >= '0' && *pc <= '9') {
			dec_val += *pc - '0';
			dec_val /= 10;
		}
		else {
			setLastError(new YInvalidCharException(__s, pc - buf, "illegal char in a number body"));
			return false;
		}
	}

	long double val = dec_val + int_val;

	pVal->pType = ptype;
	pVal->pData = new byte[pVal->pType->size];

	if (bNegative)val = -val;

	switch (ptype->size) {
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
	YType* pOldType = pType;
	byte* pOldData = pData;

	//the cases without a memory-change:
	//anyone is not a number
	//or both are int's
	if (
		pNewType->baseType != YType::cNum ||
		pOldType->baseType != YType::cNum ||
		(!((YNum*)pOldType)->bIsDecimal && !((YNum*)pNewType)->bIsDecimal)
		) {
		return new YVal(pNewType, pOldData);
	}

	//deal with the cast between int and decimal
	YVal* pNewVal = new YVal;
	pNewVal->pType = pNewType;

	if (pNewType->baseType == YType::cNum) {
		YNum* pOldNum = (YNum*)pOldType;
		YNum* pNewNum = (YNum*)pNewType;

		if (pOldNum->bIsDecimal && pNewNum->bIsDecimal) {
			//from int to dec
			long double oldval;
			switch (pOldType->size) {
			case 4:
				oldval = *(float*)pOldData;
				break;
			case 8:
				oldval = *(double*)pOldData;
				break;
			case 16:
				oldval = *(long double*)pOldData;
				break;
			}

			void* p;
			switch (pNewType->size) {
			case 4: {
				p = (float*) new byte[YType::Float->size];
				*(float*)p = oldval;
				break;
			}
			case 8: {
				p = (double*) new byte[YType::Double->size];
				*(double*)p = oldval;
				break;
			}
			case 16: {
				p = (long double*) new byte[YType::LongDouble->size];
				*(long double*)p = oldval;
				break;
			}

			}
			pNewVal->pData = (byte*)p;
		}
		else if (pOldNum->bIsDecimal) {
			//from dec to int
			long long* pll = (long long*) new byte[YType::LongLong->size];
			switch (pOldType->size) {
			case 4:
				*pll = *(float*)pOldData;
				break;
			case 8:
				*pll = *(double*)pOldData;
				break;
			case 16:
				*pll = *(long double*)pOldData;
				break;
			}
			pNewVal->pData = (byte*)pll;
		}
		else {
			//from int to dec
			long long oldval;
			switch (pOldType->size) {
			case 1:
				oldval = *(unsigned char*)pOldData;
				break;
			case 2:
				oldval = *(unsigned short*)pOldData;
				break;
			case 4:
				oldval = *(unsigned int*)pOldData;
				break;
			case 8:
				oldval = *(unsigned long long*) pOldData;
				break;
			}

			void* p;
			switch (pNewType->size) {
			case 4: {
				p = (float*) new byte[YType::Float->size];
				*(float*)p = oldval;
				break;
			}
			case 8: {
				p = (double*) new byte[YType::Double->size];
				*(double*)p = oldval;
				break;
			}
			case 16: {
				p = (long double*) new byte[YType::LongDouble->size];
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
