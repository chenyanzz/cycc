/**
 * Created by cy on 2018/5/24.
 *
 * The base class of all parts of code.
 */

#pragma once

// ReSharper disable once CppInconsistentNaming
__interface CYCC 
{
    virtual const char* className() const =0;
    virtual void print()=0;
};