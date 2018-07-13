/**
* Created by cy on 2018/5/26.
* 
* NOTE that in the tests, It SELDOM deletes pointer
*/

#include <iostream>
#include "YException.h"
#include "YVal.h"
#include "test.h"
#include "common.h"
#include "YType.h"
#include "YNumType.h"
#include "stringUtils.h"
#include "YExpression.h"
#include "YVar.h"

#define RUN(expr) try{expr;}catch(YException& e){e.print();}cout<<endl;

void test_YNumType() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;
	RUN(YNumType::parse("int")->print());
	RUN(YNumType::Char->print());
	RUN(YNumType::parse("unsigned long long")->print());
	RUN(YNumType::parse("signed short")->print());
	RUN(YNumType::parse("hia int")->print());
	RUN(YNumType::parse("int hia")->print());
	RUN(YNumType::parse("hiahia")->print());
}


void test_exception() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;

	YException("test %d %c %s", 233, 'z', "hia").print();
	YInvalidCharException("test test $", 10, "$ is illegal").print();
	YNoSuchTypeException("haha").print();
	YParseFailedException("-test-", "hia@@@", "anyway It failed").print();
}


void test_stringUtils() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;

	bool b;
	b = isFirstSubStr("abcde", "abc");
	cout << b << " ";
	b = isLastSubStr("abcde", 5, "de", 2);
	cout << b << " ";
	cout << endl;

}

void test_operator();

void test_YVal() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;

	YVal* p = nullptr;

	RUN(YVal::parse("\'Z\'")->print());
	RUN(YVal::parse("12345")->print());
	RUN(YVal::parse("-12345")->print());
	RUN(YVal::parse("12345U")->print());
	RUN(YVal::parse("12345L")->print());
	RUN(YVal::parse("12345UL")->print());
	RUN(YVal::parse("12345ULL")->print());
	RUN(YVal::parse("0xabc")->print());
	RUN(YVal::parse("0b101")->print());
	RUN(YVal::parse("0377")->print());
	RUN(YVal::parse("12345UU")->print());
	RUN(YVal::parse("abc")->print());
	RUN(YVal::parse("0xAzz")->print());
	RUN(YVal::parse("0999")->print());
	RUN(YVal::parse("!@#$%")->print());

	RUN(YVal::parse(".123")->print());
	RUN(YVal::parse("222.")->print());
	RUN(YVal::parse("102.123")->print());
	RUN(YVal::parse("1.abc")->print());


	RUN(YVal::parse("5.678")->print());

	p = YVal::parse("1.23");
	YVal* p1;
	p1 = p->clone();
	p1->print();
	delete p1;
	puts("");
	p1 = p->castTo(YNumType::Float);
	p1->print();
	delete p1;
	puts("");
	p1 = p->castTo(YNumType::LongDouble);
	p1->print();
	delete p1;
	puts("");
	p1 = p->castTo(YNumType::Int);
	p1->print();
	delete p1;
	puts("");

	p = YVal::parse("-123");
	p1 = p->castTo(YNumType::LongDouble);
	p1->print();
	puts("");

	test_operator();
}

void test_operator() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;

	YVal* p1 = YVal::parse("2");
	YVal* p2 = YVal::parse("3");

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

	p = YVal::neg(p1);
	p->print();
	puts("");
	delete p;

	delete p1, p2;

}

void test_YExpression() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;
	YVal* p = nullptr;

	auto tree = YExpression::makeTestOperationTree();
	// tree->print();
	// cout << "=" << endl;
	// p = tree->execute();
	// p->print();
	// puts("");
	delete p;
	delete tree;

	const char* s;
	cout << YExpression::parseSign(s = "+") << " ";
	cout << YExpression::parseSign(s = "-") << " ";
	cout << YExpression::parseSign(s = "*") << " ";
	cout << YExpression::parseSign(s = "/") << " ";
	RUN(cout << YExpression::parseSign(s = "#") << " ");

	endl(cout);

	cout << YExpression::getPriority(YExpression::add) << " ";
	cout << YExpression::getPriority(YExpression::sub) << " ";
	cout << YExpression::getPriority(YExpression::mul) << " ";
	cout << YExpression::getPriority(YExpression::div) << " ";

	endl(cout);

	s = "-12.34+-56.78";
	YExpression::OperationNode* node = new YExpression::OperationNode;
	node->l_operand = (YVal*)YExpression::parseIdentifier(s);
	node->opType = YExpression::parseSign(s);
	node->r_operand = (YVal*)YExpression::parseIdentifier(s);
	node->print();
	puts("");
	delete node;

	tree = YExpression::makeOperationTree("(1+2)*(3+4)"); //21
	tree->print();
	cout << "=" << endl;
	p = tree->execute();
	p->print();
	puts("");
	delete p;
	delete tree;
}

void calculator() {
	while(true) {
		try {
			cout << "> ";
			const int buf_size = 100;
			char buf[buf_size];
			cin.getline(buf, buf_size);
			if(!strcmp(buf, "exit"))break;
			YExpression::OperationNode* tree = YExpression::makeOperationTree(buf);
			tree->print();
			cout << endl;
			YVal* ans = tree->execute();
			cout << "= " << ans->castTo(YNumType::LongDouble)->data<long double>() << endl;
		} catch(YException& e) {
			cout << "cacl failed: " << e.what() << endl;
		}
	}
}

void easter_egg() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;

	for(int i = 1; i <= 10; i++) {
		printf("齐老师最帅x%d!!! ", i);
	}
	printf("\n\n岩大天使牌计算器1.0\n");
	printf("可以输入数字+-*/()回车以开始运算 \n");
	printf("输入exit 以退出 \n");
	calculator();
}

void test_YVar() {
	cout << endl << "******** " << __FUNCTION__ << " ********" << endl;

	// RUN(YVar::parse("int i1;")->print());
	// RUN(YVar::parse("long double lf1;")->print());
	// RUN(YVar::parse("hia a;")->print());
	RUN(YVar::parse("long double lf2 = 123.456;")->print());
}

void doTests() {
	YType::init();

	// test_exception();
	// test_YNumType();
	// test_stringUtils();
	// test_YVal();
	// test_YExpression();
	test_YVar();

	YType::terminate();
}
