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
		printf("����ʦ��˧x%d!!! ", i);
	}
	printf("\n\n�Ҵ���ʹ�Ƽ�����1.0\n");
	printf("������������+-*/()�س��Կ�ʼ���� \n");
	printf("emm ����������ֱ�ӱ��� \n");
	printf("����exit ���˳� \n");

	doTests();

	cout << endl << endl << "[[[[[end]]]]]" << endl;

	system("pause");
	return 0;
}
