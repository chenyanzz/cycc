/**
 * Created by cy on 2018/5/24.
 */

#include <iostream>
#include "YNum.h"

using namespace std;


const char* YNum::className() const {
    return "YNum";
}

void YNum::print() {
    cout << "[YNum]{name=\"" << name << "\"}";
}
