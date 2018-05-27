/**
 * Created by cy on 2018/5/26.
 */

#include "YVal.h"

YVal YVal::parse(char *s) {

}

YVal YVal::parseNum(char *s, bool *pbSuccess) {
    int radix = 10;
    char *pc_num = s;

    *pbSuccess = true;
    //deal with 0x 0b 0[0-7]
    if (pc_num[0] = '0') {
        pc_num++;
        switch (pc_num[1]) {
            case 'x':
                radix = 16;
                pc_num++;
                break;

            case 'b':
                radix = 2;
                pc_num++;
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
                break;

            default:
                *pbSuccess = false;
        }
    }
    unsigned long long num;
    try {
        num = toUInt(pc_num, radix);
    } catch(YException *e){
        *pbSuccess= false;
    }
    YVal val;

    //FIXME below
    val.ptr = nullptr;
}
