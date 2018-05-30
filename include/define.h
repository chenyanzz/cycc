/**
 * Created by cy on 2018/5/24.
 *
 * Include some of cycc essential #define s and #include s
 */

#pragma once

#include <algorithm>

#define cyfind(vec, item) (std::find(vec.begin(),vec.end(),item))
#define cyfind_if(vec, comp) (std::find_if(vec.begin(),vec.end(),comp))

