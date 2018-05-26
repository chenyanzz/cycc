#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class using for cycc throwing exceptions.
 */


#include "common.h"
#include <exception>
#include "stdarg.h"

class YException : public exception {
protected:
    string err_str;

public:
    YException(const char *pc_err_str, ...) ;
    const char *what();
};



