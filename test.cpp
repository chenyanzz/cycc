/**
 * Created by cy on 2018/5/26.
 */
#include <iostream>
#include "YException.h"
#include "YVal.h"
#include "test.h"
#include "common.h"
#include "YType.h"
#include "YNum.h"
#include "stringUtils.h"

#define RUN(exp,stat_print) if(exp==false){getLastError()->print();puts("");}else{stat_print;puts("");}

void test_YNum() {
	cout << endl << "******** test_YNum ********" << endl;
	YType* pType;
	RUN(YType::get("int", pType), pType->print());
	RUN(true, YType::Char->print());
	RUN(YType::get("unsigned long long", pType), pType->print());
	RUN(YType::get("signed short", pType), pType->print());

	RUN(YType::get("hia int", pType), pType->print());
	RUN(YType::get("int hia", pType), pType->print());
	RUN(YType::get("hiahia", pType), pType->print());
}


void test_exception() {
	cout << endl << "******** test_exception ********" << endl;

	YException("test %d %c %s", 233, 'z', "hia").print();
	YInvalidCharException("test test $", 10, "$ is illegal").print();
	YNoSuchTypeException("haha").print();
	YParseFailedException("-test-", "hia@@@", "anyway It failed").print();
}


void test_stringUtils() {
	cout << endl << "******** test_stringUtils ********" << endl;

	string s;
	bool b;

	s = trim("\n \n hello \n ");
	cout << s << endl;

	vector<string> vec;
	split("a;b;c;d", vec, ";");
	int p = 0;
	cout << vec[p++] << " ";
	cout << vec[p++] << " ";
	cout << vec[p++] << " ";
	cout << vec[p++] << " ";
	cout << endl;

	b = isFirstSubStr("abcde", "abc");
	cout << b << " ";
	b = isLastSubStr("abcde", 5, "de", 2);
	cout << b << " ";
	cout << endl;
}


void test_YVal() {
	cout << endl << "******** test_YVal ********" << endl;

	YVal val;

	RUN(YVal::parseInt("12345", &val), val.print());
	RUN(YVal::parseInt("-12345", &val), val.print());
	RUN(YVal::parseInt("12345U", &val), val.print());
	RUN(YVal::parseInt("12345L", &val), val.print());
	RUN(YVal::parseInt("12345UL", &val), val.print());
	RUN(YVal::parseInt("12345ULL", &val), val.print());
	RUN(YVal::parseInt("0xabc", &val), val.print());
	RUN(YVal::parseInt("0b101", &val), val.print());
	RUN(YVal::parseInt("0377", &val), val.print());
	RUN(YVal::parseInt("12345UU", &val), val.print());
	RUN(YVal::parseInt("abc", &val), val.print());
	RUN(YVal::parseInt("0xAzz", &val), val.print());
	RUN(YVal::parseInt("0999", &val), val.print());
	RUN(YVal::parseInt("!@#$%", &val), val.print());

	RUN(YVal::parseDecimal(".123", &val), val.print());
	RUN(YVal::parseDecimal("222.", &val), val.print());
	RUN(YVal::parseDecimal("102.123", &val), val.print());
	RUN(YVal::parseDecimal("1.abc", &val), val.print());

	RUN(YVal::parse("1.2", &val), val.print());


	RUN(YVal::parseDecimal("5.678", &val),);
	YVal* p = &val;

	p->clone()->print();
	puts("");
	p->castTo(YType::Float)->print();
	puts("");
	p->castTo(YType::LongDouble)->print();
	puts("");
	p->castTo(YType::Int)->print();
	puts("");
}


void doTests() {
	YType::init();

	test_exception();
	test_YNum();
	test_stringUtils();
	test_YVal();

	YType::terminate();
}