/**
 * Created by cy on 2018/5/24.
 *
 * Some functions doing with string.
 */

#pragma once

#include <string>
#include <vector>

using namespace std;

//remember to release
char* trimAll(const char* s);

bool isFirstSubStr(const char* total, const char* substr);

bool isLastSubStr(const char* total, size_t total_len, const char* substr, size_t substr_len);

char* newString(const char* begin, int len);

//returns [begin,end)
char* newString(const char* begin, const char* end);

bool isBlank(char c);

//str++ at blank chars
void skipBlank(const char*& str);

//str-- at blank chars
void skipBlankBack(const char*& str);