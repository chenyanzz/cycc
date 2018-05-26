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

bool isFirstSubStr(const char*, const char* substr);

unsigned long long toUInt(const char* s,int radix=10);