/**
 * Created by cy on 2018/5/26.
 */
#include <iostream>
#include "test.h"

#include "common.h"
#include "YType.h"
#include "YNum.h"
#include "stringUtils.h"

void test_YNum() {
    YType::init();

    YNum *types[100];
    int i = 0;

    types[i++] = (YNum *) YType::parse("int");
    types[i++] = (YNum *) YType::parse("unsigned long long");
    types[i++] = (YNum *) YType::parse("signed short");
    try {
        types[i++] = (YNum *) YType::parse("hia int");
    } catch (YException e) {
        e.print();
    }
    try {
        types[i++] = (YNum *) YType::parse("int hia");
    } catch (YException e) {
        e.print();
    }
    try {
        types[i++] = (YNum *) YType::parse("hiahia");
    } catch (YException e) {
        e.print();
    }

    YType::terminate();
}

void test_toUInt() {

    cout<<"test_toUInt:"<<endl;

    int i = 0;

    cout << toUInt("12345") <<endl;
    cout<<toUInt("25050", 10)<<endl;
    cout<<toUInt("077", 8)<<endl;
    cout<<toUInt("aBcDe", 16)<<endl;
    cout<<toUInt("10101010", 2)<<endl;

    try {
        cout<<toUInt("aBcDe", 10)<<endl;
    } catch (YException e) {
        e.print();
    }
    try {
        cout<<toUInt("1$!@@@", 10)<<endl;
    } catch (YException e) {
        e.print();
    }

}

void test_exception() {

    cout<<"test_exception:"<<endl;

    YException("test %d %c %s", 233, 'z', "hia").print();
    YInvalidCharException("test test $",10,"not a legal char in C").print();
    YNoSuchTypeException("haha").print();

}

void doTests() {
    test_exception();
    test_YNum();
    test_toUInt();
}