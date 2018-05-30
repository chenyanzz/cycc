#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class for parsing expressions kind like "(i++)", "1+1" , "int_ret_func()" etc..
 */


#include "CYCC.h"
#include "YVal.h"

class YEpxression : CYCC {

public:
    static YEpxression *parse(char *expr);

    YVal* exec();

    void print() override;
    const char* className() const override;
};



