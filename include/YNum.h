/**
 * Created by cy on 2018/5/24.
 *
 * A Sub-Type of numbers, add signed-or-not flag.
 */

#pragma once

#include "YType.h"

class YNum :YType{

public:
    bool bSigned;

    friend class YType;
};
