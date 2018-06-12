#pragma once

/**
 * Created by cy on 2018/5/26.
 *
 * A class using for cycc throwing exceptions.
 */

#include <string>
#include "CYCC.h"

class YException :public CYCC
{
protected:
	std::string err_str;

public:
	const char* className() const override;
	void print() override;

	YException(const char* errStr, ...);
	const char* what();

	virtual ~YException() = default;
};

class YInvalidCharException : public YException
{
public:
	const char* className() const override;
	YInvalidCharException(const char* src, int pos, const char* msg);
};

class YNoSuchTypeException : public YException
{
public:
	const char* className() const override;
	YNoSuchTypeException(const char* type);
};

class YParseFailedException : public YException
{
public:
	const char* className() const override;
	YParseFailedException(const char* clazz, const char* s, const char* msg);
};

class YNullptrException : public YException
{
public:
	const char* className() const override;
	YNullptrException(const char* name);
};

class YTypeNotFoundException : public YException
{
public:
	const char* className() const override;
	YTypeNotFoundException(const char* type);
};

//error dealing
YException* getLastError();
void setLastError(YException* err);
bool hasError();
void clearLastError();