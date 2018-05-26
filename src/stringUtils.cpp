/**
 * Created by cy on 2018/5/24.
 */

#include "stringUtils.h"
#include "YException.h"

string trim(const string &str) {
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos) {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos) {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}

int split(const string &str, vector<string> &ret_, string sep) {
    if (str.empty()) {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while (pos_begin != string::npos) {
        comma_pos = str.find(sep, pos_begin);
        if (comma_pos != string::npos) {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        } else {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if (!tmp.empty()) {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}

bool isFirstSubStr(const char *total, const char *substr) {

    for (int i = 0; i < substr[i] != 0; i++) {
        if (total[i] != substr[i])return false;
    }

    return true;
}

unsigned long long toUInt(const char *s, int radix) {
    unsigned long long val = 0;
    for (int i = 0; s[i] != 0; i++) {
        int hexletter = (s[i] | 0x20);
        int bitval;
        if (s[i] >= '0' && s[i] <= '9') {
            bitval = s[i] - '0';
        } else if (hexletter >= 'a' && hexletter <= 'f') {
            bitval = hexletter - 'a' + 10;
        } else {
            throw new YException(R"(in number "%s", '%c' is a illegal char)", s, s[i]);
        }

        if (bitval >= radix) {
            throw new YException(R"(in number "%s", '%c' is not a proper char for radix=%d)", s, s[i], radix);
        }

        val *= radix;
        val += bitval;
    }
    return val;
}
