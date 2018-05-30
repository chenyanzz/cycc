#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class using for cycc throwing exceptions.
 */


#include <string>
#include "define.h"
#include "CYCC.h"

class YException : CYCC {
protected:
    std::string err_str;

public:
    const char* className() const override;

    YException(const char* pc_err_str, ...);
    const char* what();
    void print() override;
};

class YInvalidCharException : public YException {
public:
    const char* className() const override;


    YInvalidCharException(const char* src, int pos, const char* msg)
            : YException("%s\n%s\n%*s", msg, src, pos + 1, "^") {};
};

class YNoSuchTypeException : public YException {
public:
    const char* className() const override;


    YNoSuchTypeException(const char* type)
            : YException("can't find the type \"%s\"", type) {};
};