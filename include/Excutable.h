﻿/**
 * Created by cy on 2018/6/10.
 *
 * The interface of classes which can execute and result an YVal*
 */

#pragma once
#include "CYCC.h"

class Executable: public CYCC {
public:
	virtual class YVal* execute() =0;
	virtual ~Executable() = default;
};
