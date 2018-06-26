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
#include "YNum.h"
#include "stringUtils.h"
#include "YExpression.h"
#include "YVar.h"

#define RUN(expr) try{expr;}catch(YException* e){e->print();}cout<<endl;

void test_YNum() {
	cout << endl << "******** test_YNum ********" << endl;
	YType* pType;
	RUN(YType::parse("int")->print());
	RUN(YType::Char->print());
	RUN(YType::parse("unsigned long long")->print());
	RUN(YType::parse("signed short")->print());
	RUN(YType::parse("hia int")->print());
	RUN(YType::parse("int hia")->print());
	RUN(YType::parse("hiahia")->print());
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

	p = YVal::parse("-123");
	p1 = p->castTo(YType::LongDouble);
	p1->print();
	puts("");
}

void test_operator() {
	cout << endl << "******** test_operator ********" << endl;

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


	delete p1, p2;

}

void test_YExpression() {
	cout << endl << "******** test_YExpression ********" << endl;
	YVal* p=nullptr;

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
	cout << YExpression::parseSign(s = "#") << " ";

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

	tree = YExpression::makeOperationTree("(1+-4)*-3"); //-3*-3=9
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
		cout << "> ";
		const int buf_size = 100;
		char buf[buf_size];
		cin.getline(buf,buf_size);
		if(!strcmp(buf,"exit"))break;
		YExpression::OperationNode* tree = YExpression::makeOperationTree(buf);
		YVal* ans = tree->execute();
		cout << "= " << ans->castTo(YType::LongDouble)->data<long double>() << endl;
	}
}

void easter_egg() {
	for(int i = 1; i <= 10; i++) {
		printf("齐老师最帅x%d!!! ", i);
	}
	printf("\n\n岩大天使牌计算器1.0\n");
	printf("可以输入数字+-*/()回车以开始运算 \n");
	printf("emm 输入其他会直接崩溃 \n");
	printf("输入exit 以退出 \n");
	calculator();
}

void test_YVar() {
	cout << endl << "******** test_YVar ********" << endl;

	YVar::parse("int i")->print();
	RUN(YVar::parse("hia a")->print());
}

void doTests() {
	YType::init();

	// test_exception();
	// test_YNum();
	// test_stringUtils();
	// test_YVal();
	// test_operator();
	// test_YExpression();
	// test_YVar();

	calculator();

	YType::terminate();
}
