/**
 * Created by cy on 2018/5/24.
 *
 * some of cycc essential's
 */

#pragma once

#include <algorithm>

typedef char byte;

#define cyfind(vec, item) (std::find((vec).begin(),(vec).end(),item))
#define cyfind_if(vec, comp) (std::find_if((vec).begin(),(vec).end(),comp))

#ifdef _DEBUG
	#define CY_DEBUG
#endif
