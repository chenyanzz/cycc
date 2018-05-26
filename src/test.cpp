/**
 * Created by cy on 2018/5/26.
 */
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
    } catch (YException *e) {
        printf("%s\n", e->what());
    }
    try {
        types[i++] = (YNum *) YType::parse("int hia");
    } catch (YException *e) {
        printf("%s\n", e->what());
    }
    try {
        types[i++] = (YNum *) YType::parse("hiahia");
    } catch (YException *e) {
        printf("%s\n", e->what());
    }

    YType::terminate();
}

void test_toUInt() {
    unsigned long long nums[100];
    int i = 0;
//    nums[i++] = toUInt("12345");
//    nums[i++] = toUInt("25050", 10);
//    nums[i++] = toUInt("077", 8);
    nums[i++] = toUInt("aBcDe", 16);
    nums[i++] = toUInt("10101010", 2);
    try {
        nums[i++] = toUInt("aBcDe", 10);
    } catch (YException *e) {
        printf("%s\n", e->what());
    }
    try {
        nums[i++] = toUInt("1$!@@@", 10);
    } catch (YException *e) {
        printf("%s\n", e->what());
    }
}


void doTests() {
    test_YNum();
    test_toUInt();
}