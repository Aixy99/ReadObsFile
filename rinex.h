#ifndef RINEX_H
#define RINEX_H
#include <stdio.h>
#include <string.h>
//常量定义
#define MAXOBSTYPE 64							//最大观测类型数量

//函数声明
extern void readObsFileH(FILE* file, char* type, char* buff);
extern void readObsFileB();
extern void readObsFile(char* file);
#endif  // !RINEX_H
