#include "tests"

const vector<string> cycc_seperators = {";", "{", "}", "(", ")", " ", "\n"};

int main() {
    const char *code = "int i=10;";

    try {
        doTests();
    } catch (YException *e) {
        printf("cycc has encountered an exception: %s", e->what());
        return -1;
    }


    return 0;
}
