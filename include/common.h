/**
 * Created by cy on 2018/5/24.
 *
 * Include some of cycc essential #define s and #include s
 */

#pragma once

#include "CYCC.h"
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "stringUtils.h"
#include "YException.h"

using namespace std;

#define cyfind(vec, item) (find(vec.begin(),vec.end(),item))
#define cyfind_if(vec, comp) (find_if(vec.begin(),vec.end(),comp))

