/**
 * Created by cy on 2018/5/26.
 */

#include "YException.h"
#include <cstdio>
#include <iostream>

using namespace std;

static char buf[100];


YException::YException(const char* pc_err_str, ...) {
    va_list vl;
    va_start(vl, pc_err_str);

    vsprintf(buf, pc_err_str, vl);
    err_str = buf;
    err_str += '\n';
    va_end(vl);
};


const char* YException::what() {
    return err_str.c_str();
}


void YException::print() {
    cout<<this->what()<<endl;
}

