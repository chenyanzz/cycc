#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class using for cycc throwing exceptions.
 */


#include "common.h"
#include <exception>
#include "stdarg.h"
#include "CYCC.h"

class YException : exception {
protected:
    string err_str;

public:
    YException(const char* pc_err_str, ...);

    const char* what();

    void print();
};

class YInvalidCharException : public YException {
public:
    YInvalidCharException(const char* src, int pos, const char* msg)
            : YException("[YInvalidCharException]: %s\n%s\n%*s", msg, src, pos+1, "^") {};
};

class YNoSuchTypeException : public YException {
public:
    YNoSuchTypeException(const char* type)
            : YException("[YNoSuchTypeException]: can't find the type \"%s\"", type) {};
};