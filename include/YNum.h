/**
 * Created by cy on 2018/5/24.
 *
 * A Sub-Type of numbers, add signed-or-not flag.
 */

#pragma once

#include "YType.h"

class YNum : public YType {

protected:
    YNum():bIsSigned(true),bIsDecimal(false){};

public:
    bool bIsSigned:1;
    bool bIsDecimal:1;//true if type is float or double

    void print() override;
    const char* className() const override;

    friend class YType;
};
