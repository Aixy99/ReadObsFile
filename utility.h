#ifndef UTILITY_H
#define UTILITY_H
#include <stdio.h>
#include "rinex.h"
//º¯ÊıÉùÃ÷
extern double str2num(const char* s, int b, int l);
extern void setstr(char* dst, const char* src, int n);
extern int str2time(const char* s, int b, int l, gtime_t* time);
extern int setSatNum(const char satCode[4]);
#endif // !UTILITY_H

