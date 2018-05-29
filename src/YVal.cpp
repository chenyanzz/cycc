/**
 * Created by cy on 2018/5/26.
 */

#include <iostream>
#include <YNum.h>
#include "YVal.h"


YVal YVal::parse(char* s) {

}


YVal* YVal::parseInt(const char* __s) {

    //for char val
    if(__s[0] == '\'' && __s[2] == '\'') {
        YVal* pval = new YVal;
        pval->ptype = YType::Char;
        pval->pdata = new char(__s[1]);
        return pval;
    }

    //for int val

    int radix = 10;
    const int buf_size = 100;
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

            case 0:
            {
                YVal* pVal = new YVal;
                pVal->ptype=YType::Int;
                pVal->pdata=new int(0);
                return pVal;
            }

            default:

                throw (YException) YInvalidCharException(buf, pc_num - buf, "illegal char in a number prefix");
        }
    }

    /*suffix*/

    YType* ptype;
    void* pdata;
    int len = (int) strlen(pc_num);
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
        throw (YException) YInvalidCharException(buf, len - 1, "suffix \"UU\" is illegal");

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
        char hexletter = (*pc_num | 0x20);
        int bitval;

        if(*pc_num >= '0' && *pc_num <= '9') {
            bitval = *pc_num - '0';
        } else if(hexletter >= 'a' && hexletter <= 'f') {
            bitval = hexletter - 'a' + 10;
        } else {
            throw (YException) YInvalidCharException(buf, pc_num - buf, "illegal char in a number body");
        }

        if(bitval >= radix) {
            throw (YException) YInvalidCharException(buf, pc_num - buf, "illegal char in a number body");
        }

        num *= radix;
        num += bitval;
    }


    /*making the YVal*/

    if(bNegative) {
        num = ~(num & ~(1 << (sizeof(num) - 1))) + 1;
    }
    YVal* pval = new YVal;
    pval->ptype = ptype;

    switch(ptype->size) {
        case 1:
            pval->pdata = new unsigned char(num);
            break;
        case 2:
            pval->pdata = new unsigned short(num);
            break;
        case 4:
            pval->pdata = new unsigned int(num);
            break;
        case 8:
            pval->pdata = new unsigned long long(num);
            break;
    }

    return pval;
}


YVal::~YVal() {
    if(ptype->baseType == YType::cNum) {
        switch(ptype->size) {
            case 1:
                delete (unsigned char*) pdata;
                break;
            case 2:
                delete (unsigned short*) pdata;
                break;
            case 4:
                delete (unsigned int*) pdata;
                break;
            case 8:
                delete (unsigned long long*) pdata;
                break;
        }
    }
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
            cout<<", type=";
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
            cout<<", type=";
            ptype->print();
            cout << "}";
        }
    }
}


YVal* YVal::parseDecimal(const char* s) {
    const int buf_size = 100;
    static char buf[buf_size];
    strcpy_s(buf, buf_size, s);
    char* pc_num = buf;
    size_t len = strlen(s);

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
       isLastSubStr(pc_num, len, "UU", 2) || isLastSubStr(pc_num, len, "uu", 2) ||
       isLastSubStr(pc_num, len, "L", 1) || isLastSubStr(pc_num, len, "l", 1)) {
        throw (YException) YInvalidCharException(buf, len - 1, "the suffix is illegal for decimal");
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
            throw (YException) YException("[YException] \"%s\" is not a proper decimal", s);
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
            throw (YException) YInvalidCharException(buf, pc - buf, "illegal char in a number body");
        }
    }

    long double val = dec_val + int_val;
    YVal* pval = new YVal;
    pval->ptype = ptype;

    switch(ptype->size) {
        case 4:
            pval->pdata = new float(val);
            break;
        case 8:
            pval->pdata = new double(val);
            break;
        case 16:
            pval->pdata = new long double(val);
            break;
    }
}
