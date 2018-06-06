/**
 * Created by cy on 2018/5/24.
 *
 * The class of C types.
 * Now it can only work with C num types.
 */

#pragma once

#include <set>
#include <string>
#include "CYCC.h"
#include "common.h"

class YType : public CYCC {

protected:

    static std::set<YType*> types;

    static const char* str_unsigned;
    static const char* str_signed;


public:
    std::string name;
    size_t size;

    enum BaseType {
        cNum, cPtr, cStruct, cEnum
    };

    BaseType baseType;

protected:
    YType() = default;

public:
    typedef YType* pYType;
    static pYType Char, UChar, Short, UShort, Int, UInt, Long, ULong, LongLong, ULongLong, Float, Double, LongDouble;

public:
    static bool add(const char* name, const int size, const BaseType baseType,YType*& pNewType);

    static bool get(const char* code,YType*& pType);

    static void init();

    static void terminate();

    const char* className() const override;
    void print() override;
};

