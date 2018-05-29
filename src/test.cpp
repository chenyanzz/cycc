/**
 * Created by cy on 2018/5/26.
 */
#include <iostream>
#include <YVal.h>
#include "test.h"

#include "common.h"
#include "YType.h"
#include "YNum.h"
#include "stringUtils.h"


void test_YNum() {
    cout << "******** test_YNum ********" << endl;

    YNum* types[100];
    int i = 0;

    YType::parse("int")->print();
    puts("");
    YType::UChar->print();
    puts("");
    YType::parse("unsigned long long")->print();
    puts("");
    YType::parse("signed short")->print();
    puts("");

    try {
        types[i++] = (YNum*) YType::parse("hia int");
    } catch(YException& e) {
        e.print();
    }
    try {
        types[i++] = (YNum*) YType::parse("int hia");
    } catch(YException& e) {
        e.print();
    }
    try {
        types[i++] = (YNum*) YType::parse("hiahia");
    } catch(YException& e) {
        e.print();
    }
}


void test_exception() {
    cout << "******** test_exception ********" << endl;

    YException("test %d %c %s", 233, 'z', "hia").print();
    YInvalidCharException("test test $", 10, "$ is illegal").print();
    YNoSuchTypeException("haha").print();

}


void test_stringUtils() {
    cout << "******** test_stringUtils ********" << endl;

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
    b;
}


void test_YVal() {
    cout << "******** test_YVal ********" << endl;

    bool bSuccess;
    //YVal::parseInt("12345")->print();
    //puts("");
    //YVal::parseInt("-12345")->print();
    //puts("");
    //YVal::parseInt("12345U")->print();
    //puts("");
    //YVal::parseInt("12345L")->print();
    //puts("");
    //YVal::parseInt("12345UL")->print();
    //puts("");
    //YVal::parseInt("12345ULL")->print();
    //puts("");
    //
    //YVal::parseInt("0xabc")->print();
    //puts("");
    //YVal::parseInt("0b101")->print();
    //puts("");
    //YVal::parseInt("0377")->print();
    //puts("");

    try {
        YVal::parseInt("12345UU")->print();
    } catch(YException& e) {
        e.print();
    }

    try {
        YVal::parseInt("abc")->print();
    } catch(YException& e) {
        e.print();
    }

    try {
        YVal::parseInt("0xAzz")->print();
    } catch(YException& e) {
        e.print();
    }

    try {
        YVal::parseInt("0999")->print();
    } catch(YException& e) {
        e.print();
    }

    try {
        YVal::parseInt("$$%**")->print();
    } catch(YException& e) {
        e.print();
    }
}


void doTests() {
    YType::init();

    //test_exception();
    //test_YNum();
    //test_stringUtils();
    test_YVal();

    YType::terminate();
}