#include <string>
#include <iostream>
#include <vector>
#include "../test.h"
#include <filesystem>
#include "YException.h"

using namespace std;

int main() {
	const char* code = "int i=10;";
	doTests();

	cout << endl << endl << "[[[[[end]]]]]" << endl;
	while(true);
	return 0;
}