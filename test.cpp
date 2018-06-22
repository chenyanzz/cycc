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
#include "YExpression.h"

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

	bool b;
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
	YVal::parse("2", p1);
	YVal* p2;
	YVal::parse("3", p2);

	YVal* p;

	p = YVal::add(p1, p2);
	p->print();
	puts("");
	delete p;

	p = YVal::sub(p1, p2);
	p->print();
	puts("");
	delete p;

	p = YVal::mul(p1, p2);
	p->print();
	puts("");
	delete p;

	p = YVal::div(p1, p2);
	p->print();
	puts("");
	delete p;


	delete p1, p2;

}

void test_YExpression() {
	YVal* p;

	auto tree = YExpression::makeTestOperationTree();
	tree->print();
	cout << "=" << endl;
	p = tree->execute();
	p->print();
	puts("");
	delete p;
	delete tree;

	const char* s;
	cout << YExpression::parseSign(s = "+") << " ";
	cout << YExpression::parseSign(s = "-") << " ";
	cout << YExpression::parseSign(s = "*") << " ";
	cout << YExpression::parseSign(s = "/") << " ";
	cout << YExpression::parseSign(s = "#") << " ";

	endl(cout);

	cout << YExpression::isCharInIdentifier('1') << " ";
	cout << YExpression::isCharInIdentifier('z') << " ";
	cout << YExpression::isCharInIdentifier('A') << " ";
	cout << YExpression::isCharInIdentifier('_') << " ";
	cout << YExpression::isCharInIdentifier('$') << " ";

	endl(cout);

	cout << YExpression::getPriority(YExpression::add) << " ";
	cout << YExpression::getPriority(YExpression::sub) << " ";
	cout << YExpression::getPriority(YExpression::mul) << " ";
	cout << YExpression::getPriority(YExpression::div) << " ";

	endl(cout);

	s = "12.34+56.78";
	YExpression::OperationNode* node = new YExpression::OperationNode;
	node->l_operand = (YVal*)YExpression::parseIdentifier(s);
	node->opType = YExpression::parseSign(s);
	node->r_operand = (YVal*)YExpression::parseIdentifier(s);
	node->print();
	puts("");
	delete node;

	tree = YExpression::makeOperationTree("1+2*3-12/4");//=1+6-3=4
	tree->print();
	cout << "=" << endl;
	p = tree->execute();
	p->print();
	puts("");
	delete p;
	delete tree;
}

void doTests() {
	YType::init();

	// test_exception();
	// test_YNum();
	// test_stringUtils();
	// test_YVal();
	//test_operator();
	test_YExpression();

	YType::terminate();
}
