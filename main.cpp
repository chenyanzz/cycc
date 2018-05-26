#include "common.h"
#include "YType.h"
#include "YNum.h"

const vector<string> cycc_seperators = {";", "{", "}", "(", ")", " ", "\n"};

void doTest_YNum() {
    YType::init();

    YNum *ptype1 = (YNum *) YType::getType("int");
    YNum *ptype2 = (YNum *) YType::getType("unsigned long long");
    YNum *ptype3 = (YNum *) YType::getType("signed short");

    YType::terminate();
}

int main() {
    const char *code = "int i=10;";

    try {
        doTest_YNum();
    } catch (YException *e) {
        printf("cycc has encountered an exception: %s", e->what());
        return -1;
    }


    return 0;
}
