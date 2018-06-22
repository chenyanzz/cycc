#include <string>
#include <iostream>
#include <vector>
#include "../test.h"
#include <filesystem>
#include "YException.h"

using namespace std;

int main() {
	const char* code = "int i=10;";
	try {
		doTests();
	}catch(YException* e) {
		e->print();
	}

	cout << endl << endl << "[[[[[end]]]]]" << endl;

	system("pause");
	return 0;
}
