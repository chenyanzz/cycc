/**
 * Created by cy on 2018/5/24.
 */

#include "stringUtils.h"
#include "YType.h"
#include "YNum.h"

#include "common.h"

set<YType *> YType::types;

const char* YType::str_unsigned = "unsigned ";
const char* YType::str_signed = "signed ";

YType *YType::addType(const char *name, const int size, const BaseType baseType) {
    YType *ptype = nullptr;

    if (baseType == cNum) {
        ptype = new YNum;
        ((YNum *) ptype)->bSigned = true;
        ptype->name = name;

        if (isFirstSubStr(name, str_unsigned)) {
            ((YNum *) ptype)->bSigned = false;
        }

        //NOTE that "signed int" and "int" is same a type
        if (isFirstSubStr(name, str_signed)) {
            ptype->name = ptype->name.c_str() + strlen(str_signed);
        }
    }

    ptype->size = size;
    ptype->baseType = baseType;


    types.insert(ptype);
    return ptype;
}

YType *YType::getType(const char *code) {

    string str_type = code;
    bool isSigned = isFirstSubStr(code, str_signed);
    if (isSigned) {
        str_type = string(code+strlen(str_signed));
    }

    auto it = cyfind_if(types, [=](YType *pt) -> bool{ pt->name == str_type; });
    if (it == types.end()){
        throw new YException(R"(no such type: "%s")", code);
    }

    if(isSigned){
        if ((*it)->baseType!=cNum){
            throw new YException(R"(no such type: "%s")", code);
        }
    }

    return *it;
}

void YType::init() {
    YType::addType("long long", 8, YType::cNum);
    YType::addType("unsigned long long", 8, YType::cNum);

    YType::addType("long", 4, YType::cNum);
    YType::addType("unsigned long", 4, YType::cNum);

    YType::addType("int", 4, YType::cNum);
    YType::addType("unsigned int", 4, YType::cNum);

    YType::addType("int", 4, YType::cNum);
    YType::addType("unsigned int", 4, YType::cNum);

    YType::addType("short", 2, YType::cNum);
    YType::addType("unsined short", 2, YType::cNum);

    YType::addType("char", 1, YType::cNum);
    YType::addType("unsigned char", 1, YType::cNum);
}

void YType::terminate() {
    for (auto it = types.begin(); it != types.end(); it++) {
        delete (*it);
    }
    types.clear();
}

