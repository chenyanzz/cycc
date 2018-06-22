/**
 * Created by cy on 2018/5/24.
 */

#include "stringUtils.h"
#include "YException.h"


char* trimAll(const char* s) {
	char* const buf = new char[strlen(s)];

	const char* p_src = s;
	char* p_buf = buf;

	do{
		switch(*p_src) {
		case '\r':
		case '\n':
		case '\t':
		case ' ':
			p_src++;
			break;

		default:
			*p_buf++ = *p_src++;
		}
	} while (*p_src != 0);

	return buf;
}

bool isFirstSubStr(const char* total, const char* substr) {
	if(total == nullptr || substr == nullptr)return false;

	for(int i = 0; substr[i] != 0; i++) {
		if(total[i] != substr[i])return false;
	}

	return true;
}


bool isLastSubStr(const char* total, const size_t total_len, const char* substr, const size_t substr_len) {
	if(total == nullptr || substr == nullptr)return false;

	for(int i = total_len - 1, j = substr_len - 1; j >= 0; i--, j--) {
		if(total[i] != substr[j])return false;
	}
	return true;
}

char* newString(const char* begin,const int len) {
	char* new_string = new char[len + 1];
	memcpy(new_string, begin, len);
	new_string[len] = 0;

	return new_string;
}

char* newString(const char* begin, const char* end) {
	return newString(begin, end - begin);
}
