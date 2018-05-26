#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class for parsing expressions kind like "(i++)", "1+1" , "int_ret_func()" etc..
 */


#include "YCode.h"
#include "YVal.h"

class YEpxression : YCode {

public:
    static YEpxression *parse(char *expr);

    YVal exec();
};



