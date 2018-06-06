/**
 * Created by cy on 2018/5/24.
 */

#include "stringUtils.h"
#include "YException.h"


string trim(const string& str) {
    string::size_type first = 0, last = str.size();

    //find first available char
    for(string::size_type i = 0; i < str.size(); i++) {
        char c = str.at(i);
        if(c != ' ' && c != '\n' && c != '\r') {
            break;
        }
        first++;
    }

    //find last available char
    for(string::size_type i = str.size()-1; i >= first; i--) {
        char c = str.at(i);
        if(c != ' ' && c != '\n' && c != '\r') {
            break;
        }
        last--;
    }

    return str.substr(first, last - first);
}


int split(const string& str, vector<string>& ret_, string sep) {
    if(str.empty()) {
        return 0;
    }

    string tmp;
    string::size_type pos_begin = str.find_first_not_of(sep);
    string::size_type comma_pos = 0;

    while(pos_begin != string::npos) {
        comma_pos = str.find(sep, pos_begin);
        if(comma_pos != string::npos) {
            tmp = str.substr(pos_begin, comma_pos - pos_begin);
            pos_begin = comma_pos + sep.length();
        } else {
            tmp = str.substr(pos_begin);
            pos_begin = comma_pos;
        }

        if(!tmp.empty()) {
            ret_.push_back(tmp);
            tmp.clear();
        }
    }
    return 0;
}


bool isFirstSubStr(const char* total, const char* substr) {
    if(total== nullptr||substr== nullptr)return false;

    for(int i = 0; substr[i] != 0; i++) {
        if(total[i] != substr[i])return false;
    }

    return true;
}


bool isLastSubStr(const char* total, const size_t total_len, const char* substr, const size_t substr_len) {
    if(total== nullptr||substr== nullptr)return false;

    for(int i = total_len - 1, j = substr_len - 1; j >= 0; i--, j--) {
        if(total[i] != substr[j])return false;
    }
    return true;
}


bool toUInt(const char* s, int radix,long long& val) {
    if(s== nullptr)return false;

    for(int i = 0; s[i] != 0; i++) {
        int hexletter = (s[i] | 0x20);
        int bitval;

        if(s[i] >= '0' && s[i] <= '9') {
            bitval = s[i] - '0';
        } else if(hexletter >= 'a' && hexletter <= 'f') {
            bitval = hexletter - 'a' + 10;
        } else if(s[i]) {

        } else {
            
        }

        if(bitval >= radix) {
			setLastError(new YInvalidCharException(s, i, "illegal char in a number"));
			return false;
        }

        val *= radix;
        val += bitval;
    }
    return val;
}
