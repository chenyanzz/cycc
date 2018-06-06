
#include <string>
#include <iostream>
#include <vector>
#include "common.h"
#include "../test.h"
#include "YException.h"

using namespace std;

const vector<string> cycc_seperators = { ";", "{", "}", "(", ")", " ", "\n" };

int main() {
	const char *code = "int i=10;";

	doTests();

	cout << endl<<endl<<"[[[[[end]]]]]" << endl;
	while (true);

	return 0;
}
