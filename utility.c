#include "utility.h"

/*���ַ���תΪ����
*args:	s		Ҫת�����ַ���
*		b		��ʼת����λ��
*		l		ת���ĳ���
* return:   ת���������
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
* ����β���޿ո���ַ���
* args:		char* dst	O	Ŀ���ַ���
*			char* src	I	Դ�ַ���
*			int n		I	Ҫ���Ƶĳ���
* return:	��
*/
extern void setstr(char* dst, const char* src, int n) {
    char* p = dst;    // ָ��Ŀ���ַ�����ָ��
    const char* q = src;    // ָ��Դ�ַ�����ָ��

    // q < src + n ȷ�� q ������ src ��ǰ n ���ַ���
    while (*q && q < src + n) *p++ = *q++;    // ��Դ�ַ����е��ַ����Ƶ�Ŀ���ַ�����
    *p-- = '\0';    // ������
    while (p >= dst && *p == ' ') *p-- = '\0'; // ɾ��β��ո�
}