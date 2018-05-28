/**
 * Created by cy on 2018/5/26.
 */

#include <iostream>
#include "YVal.h"


YVal YVal::parse(char* s) {

    //    YType* ptype = YType::parse();
    //
    //    YVal val;
    //    val.ptype = ptype;
    //
    //    bool bSuccess;
    //
    //    parseInt(s, &bSuccess);
    //    if(bSuccess) {
    //
    //    }

}


YVal* YVal::parseInt(const char* s, bool* pbSuccess) {

    int radix = 10;
    const int buf_size = 100;
    static char buf[buf_size];
    strcpy_s(buf, buf_size, s);
    char* pc_num = buf;

    bool bNegative = false;
    *pbSuccess = true;

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

            default:
                *pbSuccess = false;
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
        } else if(*pc_num) {

        } else {
            throw (YException) YInvalidCharException(s, pc_num - s, "illegal char in a number");
        }

        if(bitval >= radix) {
            throw (YException) YInvalidCharException(s, pc_num - s, "illegal char in a number");
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
        ptype->print();
        cout << "}";
    }
}
