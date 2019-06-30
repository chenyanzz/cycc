/**
 * Created by cy on 2018/5/24.
 *
 * A Sub-Type of numbers, add flags.
 */

#pragma once

#include "YType.h"

class YNumType: public YType {
protected:
	//!@param name of type "unsigned A" is "A"
	YNumType(const char* name, size_t size, bool bIsSigned, bool bIsDecimal);

public:
	typedef YNumType* pYNumType;
	static pYNumType Char, UChar, Short, UShort, Int, UInt, Long, ULong, LongLong, ULongLong, Float, Double, LongDouble;

public:
	bool bIsDecimal:1; //be true if type is float or double
	bool bIsSigned:1;

public:
	void print() override;
	const char* className() const override;

	friend class YType;
	static void init();

	virtual ~YNumType() = default;

	bool isInteger();
};
