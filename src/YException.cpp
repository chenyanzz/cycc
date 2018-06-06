/**
 * Created by cy on 2018/5/26.
 */

#include <cstdio>
#include <iostream>
#include <cstdarg>
#include "YException.h"

using namespace std;

static char buf[100];

static YException* pLastError = nullptr;

YException* getLastError() {
	return pLastError;
}

void clearLastError() {
	setLastError(nullptr);
}

void setLastError(YException* err) {
	if (pLastError != nullptr)delete pLastError;
	pLastError = err;
}

bool hasError() {
	return pLastError != nullptr;
}

YException::YException(const char* pc_err_str, ...) {
	va_list vl;
	va_start(vl, pc_err_str);

	vsprintf(buf, pc_err_str, vl);
	err_str = buf;

	va_end(vl);
};


const char* YException::what() {
	return err_str.c_str();
}


void YException::print() {
	cout << endl << "[" << className() << "] encountered:" << endl;
	cout << this->what() << endl << endl;
}


const char* YException::className() const { return "YException"; }


const char* YInvalidCharException::className() const { return "YInvalidCharException"; }


YInvalidCharException::YInvalidCharException(const char* src, int pos, const char* msg)
	: YException("%s\n%s\n%*s", msg, src, pos + 1, "^") {}


const char* YNoSuchTypeException::className() const { return "YNoSuchTypeException"; }


YNoSuchTypeException::YNoSuchTypeException(const char* type)
	: YException("can't find the type \"%s\"", type) {}


const char* YParseFailedException::className() const { return "YParseFailedException"; };


YParseFailedException::YParseFailedException(const char* clazz, const char* s, const char* msg)
	: YException("[%s] cannot parse \"%s\":\n%s", clazz, s, msg) {};

const char * YNullptrException::className() const
{
	return "YNullptrException";
}

YNullptrException::YNullptrException(const char * name)
	:YException("\"%s\" cannot be nullptr", name) {}

const char * YTypeNotFoundException::className() const
{
	return "YTypeNotFoundException";
}

YTypeNotFoundException::YTypeNotFoundException(const char * type):YException("type \"%s\" not found",type){}
