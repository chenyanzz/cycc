/**
 * Created by cy on 2018/5/24.
 *
 * The class of C types.
 * Now it can only work with C num types.
 */

#pragma once

#include "CYCC.h"
#include "common.h"


class YType : CYCC {

protected:

    static set<YType *> types;

    static const char *str_unsigned;
    static const char *str_signed;


public:
    string name;
    int size;

    enum BaseType {
        cNum, cPtr, cStruct, cEnum
    };

    BaseType baseType;

protected:
    YType()= default;

public:
    typedef YType* pYType;
    static pYType Char,UChar,Short,UShort,Int,UInt,Long,ULong,LongLong,ULongLong,Float,Double;

public:
    static YType *add(const char *name, const int size, const BaseType baseType);

    static YType *parse(const char *code);

    static void init();

    static void terminate();

    void print() override ;
};

