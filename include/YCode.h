/**
 * Created by cy on 2018/5/24.
 *
 * The base class of all parts of code.
 */

#pragma once

#include "common.h"

class YCode {
public:
    static void init(){
        throw new YException("YCode::init() called");
    };
    static void terminate(){
        throw new YException("YCode::terminate() called");
    };
};