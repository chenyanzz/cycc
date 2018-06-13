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

	YVal* p = nullptr;

	RUN(YVal::parse("\'Z\'", p), p->print());
	delete p;
	RUN(YVal::parse("12345",p),p->print());
	delete p;
	RUN(YVal::parse("-12345",p),p->print());
	delete p;
	RUN(YVal::parse("12345U",p),p->print());
	delete p;
	RUN(YVal::parse("12345L",p),p->print());
	delete p;
	RUN(YVal::parse("12345UL",p),p->print());
	delete p;
	RUN(YVal::parse("12345ULL",p),p->print());
	delete p;
	RUN(YVal::parse("0xabc",p),p->print());
	delete p;
	RUN(YVal::parse("0b101",p),p->print());
	delete p;
	RUN(YVal::parse("0377",p),p->print());
	delete p;
	RUN(YVal::parse("12345UU",p),p->print());
	delete p;
	RUN(YVal::parse("abc",p),p->print());
	delete p;
	RUN(YVal::parse("0xAzz",p),p->print());
	delete p;
	RUN(YVal::parse("0999",p),p->print());
	delete p;
	RUN(YVal::parse("!@#$%",p),p->print());
	delete p;

	RUN(YVal::parse(".123",p),p->print());
	delete p;
	RUN(YVal::parse("222.",p),p->print());
	delete p;
	RUN(YVal::parse("102.123",p),p->print());
	delete p;
	RUN(YVal::parse("1.abc",p),p->print());
	delete p;

	RUN(YVal::parse("5.678", p),);
	YVal* p1;
	p1 = p->clone();
	p1->print();
	delete p1;
	puts("");
	p1 = p->castTo(YType::Float);
	p1->print();
	delete p1;
	puts("");
	p1 = p->castTo(YType::LongDouble);
	p1->print();
	delete p1;
	puts("");
	p1 = p->castTo(YType::Int);
	p1->print();
	delete p1;
	puts("");


}

void test_operator() {
	YVal* p1;
	YVal::parse("1.2", p1);
	YVal* p2;
	YVal::parse("1.3", p2);

	YVal::add(p1, p2)->print();
	puts("");
	YVal::mul(p1, p2)->print();
	puts("");

}

void doTests() {
	YType::init();

	// test_exception();
	// test_YNum();
	// test_stringUtils();
	// test_YVal();
	test_operator();

	YType::terminate();
}
