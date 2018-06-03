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


const char* YVal::className() const {
    return "YVal";
}


void YVal::print() {
    if(ptype->baseType == YType::cNum) {
        YNum* pNum = (YNum*) ptype;
        if(pNum->bIsDecimal) {
            cout << "(YVal){num=";
            switch(ptype->size) {
                case 4:
                    cout << *(float*) pdata;
                    break;
                case 8:
                    cout << *(double*) pdata;
                    break;
                case 16:
                    cout << *(long double*) pdata;
                    break;
            }
            cout << ", type=";
            ptype->print();
            cout << "}";
        } else {
            cout << "(YVal){data=0x";
            switch(ptype->size) {
                case 1:
                    cout << hex << *(unsigned char*) pdata;
                    break;
                case 2:
                    cout << hex << *(unsigned short*) pdata;
                    break;
                case 4:
                    cout << hex << *(unsigned int*) pdata;
                    break;
                case 8:
                    cout << hex << *(unsigned long long*) pdata;
                    break;
            }
            cout << ", type=";
            ptype->print();
            cout << "}";
        }
    }
}


YVal* YVal::parse(char* s) {
    try { return parseDecimal(s); } catch(YParseFailedException& e) {}
    try { return parseInt(s); } catch(YParseFailedException& e) {}

    throw YParseFailedException("YVal",s,"not a proper literal");
}


YVal* YVal::parseInt(const char* __s) {

    //for char val
    if(__s[0] == '\'' && __s[2] == '\'') {
        YVal* pval = new YVal;
        pval->ptype = YType::Char;
        pval->pdata = new byte[pval->ptype->size];
        *(char*) pval->pdata = __s[1];
        return pval;
    }

    //for int val

    int radix = 10;
    const size_t buf_size = 100;
    static char buf[buf_size];
    strcpy_s(buf, buf_size, __s);
    char* pc_num = buf;

    bool bNegative = false;

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

            case 0: {
                YVal* pVal = new YVal;
                pVal->ptype = YType::Int;
                pVal->pdata = new byte[pVal->ptype->size];
                *(int*) pVal->pdata = 0;
                return pVal;
            }

            default:

                throw YInvalidCharException(__s, pc_num - buf, "illegal char in a number prefix");
        }
    }

    /*suffix*/

    YType* ptype;
    void* pdata;
    size_t len = min(strlen(pc_num), buf_size - (pc_num - buf));

    if(isLastSubStr(pc_num, len, "ULL", 3) || isLastSubStr(pc_num, len, "ull", 3)) {
        len -= 3;
        ptype = YType::ULongLong;
    } else if(isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2)) {
        len -= 2;
        ptype = YType::ULong;
    } else if(isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2)) {
        len -= 2;
        ptype = YType::LongLong;
    } else if(isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {
        throw YInvalidCharException(__s, len - 1, "suffix \"UU\" is illegal");

    } else if(isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
        len--;
        ptype = YType::Long;
    } else if(isLastSubStr(pc_num, len, "U", 1) || isLastSubStr(pc_num, len, "u", 1)) {
        len--;
        ptype = YType::UInt;
    } else {
        ptype = YType::Int;
    }

    /*number part*/

    unsigned long long num = 0;
    for(char* org_p = pc_num; pc_num - org_p < len; pc_num++) {
        char hexletter = (*pc_num | (char) 0x20);
        int bitval;

        if(*pc_num >= '0' && *pc_num <= '9') {
            bitval = *pc_num - '0';
        } else if(hexletter >= 'a' && hexletter <= 'f') {
            bitval = hexletter - 'a' + 10;
        } else {
            throw YInvalidCharException(__s, pc_num - buf, "illegal char in a number body");
        }

        if(bitval >= radix) {
            throw YInvalidCharException(__s, pc_num - buf, "illegal char in a number body");
        }

        num *= radix;
        num += bitval;
    }


    /*making the YVal*/

    if(bNegative) {
        num = ~(num & ~(1 << (sizeof(num) - 1))) + 1;
    }
    YVal* pVal = new YVal;
    pVal->ptype = ptype;
    pVal->pdata = new byte[pVal->ptype->size];

    switch(ptype->size) {
        case 1:
            *(unsigned char*) pVal->pdata = num;
            break;
        case 2:
            *(unsigned short*) pVal->pdata = num;
            break;
        case 4:
            *(unsigned int*) pVal->pdata = num;
            break;
        case 8:
            *(unsigned long long*) pVal->pdata = num;
            break;
    }

    return pVal;
}


YVal::~YVal() {
    if(pdata == nullptr)return;

    delete[] pdata;
    pdata = nullptr;
}


YVal* YVal::parseDecimal(const char* __s) {
    if(!strcmp(__s, ".")) {
        throw YException("[YException] \"%s\" is not a proper decimal", __s);
    }

    const size_t buf_size = 100;
    static char buf[buf_size];
    strcpy_s(buf, buf_size, __s);
    char* pc_num = buf;
    size_t len = min(strlen(__s), buf_size);

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
    YType* ptype = nullptr;
    if(isLastSubStr(pc_num, len, "ULL", 3) || isLastSubStr(pc_num, len, "ull", 3) ||
       isLastSubStr(pc_num, len, "UL", 2) || isLastSubStr(pc_num, len, "ul", 2) ||
       isLastSubStr(pc_num, len, "LL", 2) || isLastSubStr(pc_num, len, "ll", 2) ||
       isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2)) {
        throw YInvalidCharException(__s, len - 1, "the suffix is illegal for decimal");
    }

    ptype = YType::Double;

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
    *pc_dot = 0;

    long long int_val = (pc_dot == pc_num) ? 0 : *(long long*) parseInt(buf)->pdata;

    /*decimal part*/

    long double dec_val = 0;

    for(char* pc = buf + len - 1; *pc != 0; pc--) {
        if(*pc >= '0' && *pc <= '9') {
            dec_val += *pc - '0';
            dec_val /= 10;
        } else {
            throw YInvalidCharException(__s, pc - buf, "illegal char in a number body");
        }
    }

    long double val = dec_val + int_val;
    YVal* pVal = new YVal;
    pVal->ptype = ptype;
    pVal->pdata = new byte[pVal->ptype->size];

    switch(ptype->size) {
        case 4:
            *(float*) pVal->pdata = val;
            break;
        case 8:
            *(double*) pVal->pdata = val;
            break;
        case 16:
            *(long double*) pVal->pdata = val;
            break;
    }
}


YVal* YVal::castTo(YType* pNewType) {
    YType* pOldType = ptype;
    byte* pOldData = pdata;

    //the cases without a memory-change:
    //anyone is not a number
    //or both are int's
    if(
            pNewType->baseType != YType::cNum ||
            pOldType->baseType != YType::cNum ||
            (!((YNum*) pOldType)->bIsDecimal && !((YNum*) pNewType)->bIsDecimal)
            ) {
        return new YVal(pNewType, pOldData);
    }

    //deal with the cast between int and decimal
    YVal* pNewVal = new YVal;
    pNewVal->ptype = pNewType;

    if(pNewType->baseType == YType::cNum) {
        YNum* pOldNum = (YNum*) pOldType;
        YNum* pNewNum = (YNum*) pNewType;

        if(pOldNum->bIsDecimal && pNewNum->bIsDecimal) {
            //from int to dec
            long double oldval;
            switch(pOldType->size) {
                case 4:
                    oldval = *(float*) pOldData;
                    break;
                case 8:
                    oldval = *(double*) pOldData;
                    break;
                case 16:
                    oldval = *(long double*) pOldData;
                    break;
            }

            void* p;
            switch(pNewType->size) {
                case 4: {
                    p = (float*) new byte[YType::Float->size];
                    *(float*) p = oldval;
                    break;
                }
                case 8: {
                    p = (double*) new byte[YType::Double->size];
                    *(double*) p = oldval;
                    break;
                }
                case 16: {
                    p = (long double*) new byte[YType::LongDouble->size];
                    *(long double*) p = oldval;
                    break;
                }

            }
            pNewVal->pdata = (byte*) p;
        } else if(pOldNum->bIsDecimal) {
            //from dec to int
            long long* pll = (long long*) new byte[YType::LongLong->size];
            switch(pOldType->size) {
                case 4:
                    *pll = *(float*) pOldData;
                    break;
                case 8:
                    *pll = *(double*) pOldData;
                    break;
                case 16:
                    *pll = *(long double*) pOldData;
                    break;
            }
            pNewVal->pdata = (byte*) pll;
        } else {
            //from int to dec
            long long oldval;
            switch(pOldType->size) {
                case 1:
                    oldval = *(unsigned char*) pOldData;
                    break;
                case 2:
                    oldval = *(unsigned short*) pOldData;
                    break;
                case 4:
                    oldval = *(unsigned int*) pOldData;
                    break;
                case 8:
                    oldval = *(unsigned long long*) pOldData;
                    break;
            }

            void* p;
            switch(pNewType->size) {
                case 4: {
                    p = (float*) new byte[YType::Float->size];
                    *(float*) p = oldval;
                    break;
                }
                case 8: {
                    p = (double*) new byte[YType::Double->size];
                    *(double*) p = oldval;
                    break;
                }
                case 16: {
                    p = (long double*) new byte[YType::LongDouble->size];
                    *(long double*) p = oldval;
                    break;
                }

            }
            pNewVal->pdata = (byte*) p;
        }
    }

    return pNewVal;

}


YVal* YVal::clone() {
    return new YVal(this);
}


YVal::YVal(YType* ptype, void* pdata) {
    this->ptype = ptype;
    this->pdata = new byte[this->ptype->size];

    memset(this->pdata, 0, this->ptype->size);
    memcpy(this->pdata, pdata, min(this->ptype->size, ptype->size));
}


YVal::YVal(YVal* pVal) : YVal(pVal->ptype, pVal->pdata) {}
