/**
 * Created by cy on 2018/5/26.
 */

#include "YVal.h"

YVal YVal::parse(char *s) {

}

YVal YVal::parseNum(char *s, bool *pbSuccess) {
    int radix = 10;
    *pbSuccess = true;
    //deal with 0x 0b 0[0-7]
    if (s[0] = '0') {
        switch (s[1]) {
            case 'x':
                radix = 16;
                break;

            case 'b':
                radix = 2;
                break;

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
                radix = 8;

            default:
                *pbSuccess= false;
                return YVal();
        }
    }
}
