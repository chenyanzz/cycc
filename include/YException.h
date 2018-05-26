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

    YException(const char *pc_err_str, ...) {
        va_list vl;
        va_start(vl, pc_err_str);

        char ps[100];
        vsprintf(ps, pc_err_str, vl);
        err_str = ps;

        va_end(vl);
    };

    const char *what() {
        return err_str.c_str();
    }
};



