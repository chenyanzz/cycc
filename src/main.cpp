#include <string>
#include <iostream>
#include <vector>
#include "../test.h"
#include <filesystem>

using namespace std;
using namespace std::experimental::filesystem;

int main() {
	const char* code = "int i=10;";

	for(int i=1;i<=10;i++) {
		printf("齐老师最帅x%d!!! ", i);
	}
	printf("\n\n岩大天使牌计算器1.0\n");
	printf("可以输入数字+-*/()回车以开始运算 \n");
	printf("emm 输入其他会直接崩溃 \n");
	printf("输入exit 以退出 \n");

	doTests();

	cout << endl << endl << "[[[[[end]]]]]" << endl;

	system("pause");
	return 0;
}
