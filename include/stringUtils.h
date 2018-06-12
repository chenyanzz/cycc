/**
 * Created by cy on 2018/5/24.
 *
 * Some functions doing with string.
 */

#pragma once

#include <string>
#include <vector>

using namespace std;

string trim(const string& str);

int split(const string& str, vector<string>& ret_, string sep);

bool isFirstSubStr(const char* total, const char* substr);

bool isLastSubStr(const char* total, size_t total_len, const char* substr, size_t substr_len);