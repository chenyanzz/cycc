#pragma once

/**
 * Created by cy on 2018/5/26.
 */

#include "YType.h"

class YVal {
public:
    static YVal parse(char *s);

protected:
    YType* ptype;
    void* ptr;

    YVal(){};

    static YVal parseNum(char *s, bool *bSuccess);

};



