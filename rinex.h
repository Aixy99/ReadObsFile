#ifndef RINEX_H
#define RINEX_H
#include <stdio.h>
#include <string.h>
//��������
#define MAXOBSTYPE 64							//���۲���������

//��������
extern void readObsFileH(FILE* file, char* type, char* buff);
extern void readObsFileB();
extern void readObsFile(char* file);
#endif  // !RINEX_H
