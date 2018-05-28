/**
 * Created by cy on 2018/5/24.
 */

#include <iostream>
#include "stringUtils.h"
#include "YType.h"
#include "YNum.h"

#include "common.h"

set<YType*> YType::types;

const char* YType::str_unsigned = "unsigned ";

const char* YType::str_signed = "signed ";

YType::pYType
        YType::Char= nullptr,
        YType::UChar= nullptr,
        YType::Short= nullptr,
        YType::UShort= nullptr,
        YType::Int= nullptr,
        YType::UInt= nullptr,
        YType::Long= nullptr,
        YType::ULong= nullptr,
        YType::LongLong= nullptr,
        YType::ULongLong= nullptr,
        YType::Float= nullptr,
        YType::Double= nullptr;


YType* YType::add(const char* name, const int size, const BaseType baseType) {
    YType* ptype = nullptr;

    if(baseType == cNum) {
        ptype = new YNum;
        ((YNum*) ptype)->bIsSigned = true;
        ptype->name = name;

        if(isFirstSubStr(name, str_unsigned)) {
            ((YNum*) ptype)->bIsSigned = false;
        }

        //NOTE that "signed int" and "int" is same a type
        if(isFirstSubStr(name, str_signed)) {
            ptype->name = ptype->name.c_str() + strlen(str_signed);
        }
    }

    ptype->size = size;
    ptype->baseType = baseType;


    types.insert(ptype);
    return ptype;
}


YType* YType::parse(const char* code) {

    string str_type = code;
    bool isSigned = isFirstSubStr(code, str_signed);
    if(isSigned) {
        str_type = string(code + strlen(str_signed));
    }

    auto it = cyfind_if(types, [=](YType* pt) -> bool { pt->name == str_type; });
    if(it == types.end()) {
        throw (YException) YNoSuchTypeException(code);
    }

    if(isSigned) {
        if((*it)->baseType != cNum) {
            throw (YException) YNoSuchTypeException(code);
        }
    }

    return *it;
}


void YType::init() {
    LongLong = YType::add("long long", 8, YType::cNum);
    ULongLong = YType::add("unsigned long long", 8, YType::cNum);

    Long = YType::add("long", 4, YType::cNum);
    ULong = YType::add("unsigned long", 4, YType::cNum);

    Int = YType::add("int", 4, YType::cNum);
    UInt = YType::add("unsigned int", 4, YType::cNum);

    Short = YType::add("short", 2, YType::cNum);
    UShort = YType::add("unsined short", 2, YType::cNum);

    Char = YType::add("char", 1, YType::cNum);
    UChar = YType::add("unsigned char", 1, YType::cNum);

    Float = YType::add("float", 4, YType::cNum);
    Double = YType::add("double", 8, YType::cNum);
}


void YType::terminate() {
    for(auto it = types.begin(); it != types.end(); it++) {
        delete (*it);
    }
    types.clear();
}


void YType::print() {
    cout<<"(YType){name=\""<<name<<"\", size="<<size<<", baseType="<<baseType<<"}";
}

