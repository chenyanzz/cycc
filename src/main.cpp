
#include <string>
#include <vector>
#include "define.h"
#include "test.h"
#include "YException.h"

using namespace std;

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
