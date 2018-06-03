/**
 * Created by cy on 2018/5/26.
 */
#include <iostream>
#include "YException.h"
#include "YVal.h"
#include "test.h"
#include "define.h"
#include "YType.h"
#include "YNum.h"
#include "stringUtils.h"


void test_YNum() {
    cout << endl << "******** test_YNum ********" << endl;

    YType::get("int")->print();
    puts("");
    YType::UChar->print();
    puts("");
    YType::get("unsigned long long")->print();
    puts("");
    YType::get("signed short")->print();
    puts("");

    try { YType::get("hia int")->print(); } catch(YException& e) { e.print(); }
    try { YType::get("int hia")->print(); } catch(YException& e) { e.print(); }
    try { YType::get("hiahia")->print(); } catch(YException& e) { e.print(); }
}


void test_exception() {
    cout << endl << "******** test_exception ********" << endl;

    YException("test %d %c %s", 233, 'z', "hia").print();
    YInvalidCharException("test test $", 10, "$ is illegal").print();
    YNoSuchTypeException("haha").print();
    YParseFailedException("-test-","hia@@@","anyway It failed").print();
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

    YVal::parseInt("12345")->print();
    puts("");
    YVal::parseInt("-12345")->print();
    puts("");
    YVal::parseInt("12345U")->print();
    puts("");
    YVal::parseInt("12345L")->print();
    puts("");
    YVal::parseInt("12345UL")->print();
    puts("");
    YVal::parseInt("12345ULL")->print();
    puts("");

    YVal::parseInt("0xabc")->print();
    puts("");
    YVal::parseInt("0b101")->print();
    puts("");
    YVal::parseInt("0377")->print();
    puts("");

    YVal::parseDecimal(".123")->print();
    puts("");
    YVal::parseDecimal("0.123")->print();
    puts("");
    YVal::parseDecimal("1.123")->print();
    puts("");

    try { YVal::parseInt("12345UU")->print(); } catch(YException& e) { e.print(); }

    try { YVal::parseInt("abc")->print(); } catch(YException& e) { e.print(); }

    try { YVal::parseInt("0xAzz")->print(); } catch(YException& e) { e.print(); }

    try { YVal::parseInt("0999")->print(); } catch(YException& e) { e.print(); }

    try { YVal::parseInt("$$%**")->print(); } catch(YException& e) { e.print(); }

    try { YVal::parseDecimal("1.abc")->print(); } catch(YException& e) { e.print(); }

    YVal* p = YVal::parseDecimal("123.456");
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