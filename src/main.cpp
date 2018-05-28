
#include <common.h>
#include "test.h"

const vector<string> cycc_seperators = {";", "{", "}", "(", ")", " ", "\n"};

int main() {
    const char *code = "int i=10;";

    try { 
        doTests();
    } catch (YException& e) {
        e.print();
        return -1;
    }


    return 0;
}
