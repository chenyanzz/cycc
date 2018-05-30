/**
 * Created by cy on 2018/5/24.
 *
 * The base class of all parts of code.
 */

#pragma once

#include <iostream>
#include "define.h"

class CYCC {
public:
    virtual const char* className() const =0;


    virtual void print()=0;
};