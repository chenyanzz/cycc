/**
 * Created by cy on 2018/5/26.
 */

#include <cstdio>
#include <iostream>
#include <cstdarg>
#include <string>
#include "YException.h"

using namespace std;

//for sprintf() in exceptions' ctors
static const size_t buf_size=100;
static char buf[buf_size];


const char* YException::className() const { return "YException"; }
YException::YException(const char* pc_err_str, ...) {
	va_list vl;
	va_start(vl, pc_err_str);

	vsprintf(buf, pc_err_str, vl);
	err_str = buf;

	va_end(vl);
};

const string YException::what() { 
	err_str = string()+"@"+className()+":\n"+err_str;
	return err_str;
}

void YException::print() {
	cout << endl << "[" << className() << "] encountered:" << endl;
	cout << this->what() << endl << endl;
}

//YInvalidCharException
const char* YInvalidCharException::className() const { return "YInvalidCharException"; }

YInvalidCharException::YInvalidCharException(const char* src, size_t pos, const char* msg)
	: YException("%s\n%s\n%*s", msg, src, pos + 1, "^") {}

//YNoSuchTypeException
const char* YNoSuchTypeException::className() const { return "YNoSuchTypeException"; }

YNoSuchTypeException::YNoSuchTypeException(const char* type)
	: YException("can't find the type \"%s\"", type) {}

//YParseFailedException
const char* YParseFailedException::className() const { return "YParseFailedException"; };

YParseFailedException::YParseFailedException(const char* clazz, const char* s, const char* msg)
	: YException("[%s] cannot parse \"%s\":\n%s", clazz, s, msg) {};

//YNullptrException
const char * YNullptrException::className() const{return "YNullptrException";}

YNullptrException::YNullptrException(const char * name)
	:YException("\"%s\" cannot be nullptr", name) {}

//YTypeNotFoundException
const char * YTypeNotFoundException::className() const{return "YTypeNotFoundException";}

YTypeNotFoundException::YTypeNotFoundException(const char * type) 
	:YException("type \"%s\" not found", type) {}

const char* YNotAAvailableNameException::className() const { return "YNotAAvailableNameException"; }

YNotAAvailableNameException::YNotAAvailableNameException(const char* name)
	:YException("\"%s\" is not a available name",name){}

//YDividedByZeroException
const char* YDividedByZeroException::className() const{return "YDividedByZeroException";}

YDividedByZeroException::YDividedByZeroException()
	:YException("divided by zero"){}