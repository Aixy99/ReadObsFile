#include "utility.h"

/*将字符串转为数字
*args:	s		要转换的字符串
*		b		开始转换的位置
*		l		转换的长度
* return:   转换后的数字
*/
extern double str2num(const char* s, int b, int l) {
    double val = 0;
    char str[256], * p = str;

    for (s += b; *s && (--l >= 0);) {
        *p++ = *s++;
    }
    *p = 0;
    sscanf(str, "%lf", &val);
    return val;
}

/*
* 设置尾部无空格的字符串
* args:		char* dst	O	目标字符串
*			char* src	I	源字符串
*			int n		I	要复制的长度
* return:	无
*/
extern void setstr(char* dst, const char* src, int n) {
    char* p = dst;    // 指向目标字符串的指针
    const char* q = src;    // 指向源字符串的指针

    // q < src + n 确保 q 不超过 src 的前 n 个字符。
    while (*q && q < src + n) *p++ = *q++;    // 将源字符串中的字符复制到目标字符串中
    *p-- = '\0';    // 结束符
    while (p >= dst && *p == ' ') *p-- = '\0'; // 删除尾随空格
}