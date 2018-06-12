/**
* Created by cy on 2018/6/10.
*
* The interface of classes which can execute and result an YVal*
*/

#pragma once
#include "CYCC.h"
#include "YVal.h"

// ReSharper disable once CppInconsistentNaming
__interface Executable : CYCC
{
	virtual YVal* execute() = 0;
};