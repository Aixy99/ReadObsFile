#include "utility.h"
#include "rinex.h"

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

/*
* 将时间戳转换为gtime_t格式
*
* args:		double* ep		I		时间戳
*
* return:	转换后的gtime_t时间
*/
static gtime_t epoch2time(const double* ep) {
    gtime_t t = { 0 };
    const int doy[] = { 1, 32, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };    // (day of year) 每个月份的开始天数
    int year = (int)ep[0];
    int month = (int)ep[1];
    int day = (int)ep[2];
    int days;    // 总天数
    int sec;    // 秒

    days = (year - 1970) * 365 + numLeap(1970, year) + doy[month - 1] + day - 2 + (isLeap(year) && month >= 3 ? 1 : 0);
    sec = (int)floor(ep[5]);    // 向下取整
    t.time += days * 86400 + ep[3] * 3600 + ep[4] * 60 + sec;
    t.sec = ep[5] - sec;

    return t;
}

/*
* 判断是否为闰年
*
* args:		int y		I		年份
*
* return:	0:平年  1:闰年
*/
static int isLeap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0;
}

/*
* 计算闰年个数
* args:		int begin		I		开始年份
*			int end			I		结束年份
*
* return:	闰年的个数
*/
static int numLeap(const int begin, const int end) {
    int count = 0;
    for (int i = begin; i < end; i++) {
        if (isLeap(i)) count++;
    }
    return count;
}

/*
* 将字符串转为gtime_t格式
*
* args:		char* buff		I		字符串
*			int b			I		从字符串第b个开始读取(begin)
*			int l			I		要读取的长度(length)
*			gtime_t			O		保存
*
* return:	成功返回1，失败返回0
*
* 要转换的时间字符串格式(YYYY MM DD HH MM SS.SSSSSSSS)
*/
extern int str2time(const char* s, int b, int l, gtime_t* time) {
    double eq[6];                        // 储存分量
    char str[256], * p = str;            // 用来保存要读取的字符串

    for (s += b; *s && (--l >= 0);) {    //保存表示时间的字符串
        *p++ = *s++;
    }
    *p = 0;
    if (!(sscanf(str, "%lf %lf %lf %lf %lf %lf", eq, eq + 1, eq + 2, eq + 3, eq + 4, eq + 5))) return 0;

    *time = epoch2time(eq);                // 将时间戳转换为gtime_t格式
    return 1;
}

/*设置卫星编号*/
extern int setSatNum(const char satCode[4]) {
    char sys = satCode[0];
    int prn = str2num(satCode, 1, 2);
    int sat;
    
    switch (sys) {
        case 'G':   sat = MAXGPSNUM + prn; break;
        case 'R':   sat = MAXGPSNUM + MAXGLONUM + prn; break;
        case 'E':   sat = MAXGPSNUM + MAXGLONUM + MAXGALNUM + prn; break;
        case 'J':   sat = MAXGPSNUM + MAXGLONUM + MAXGALNUM + MAXQZSNUM + prn; break;
        case 'S':   sat = MAXGPSNUM + MAXGLONUM + MAXGALNUM + MAXQZSNUM + MAXSBSNUM + prn; break;
        case 'C':   sat = MAXGPSNUM + MAXGLONUM + MAXGALNUM + MAXQZSNUM + MAXSBSNUM + MAXBDSNUM + prn; break;
        case 'I':   sat = MAXGPSNUM + MAXGLONUM + MAXGALNUM + MAXQZSNUM + MAXSBSNUM + MAXBDSNUM + MAXIRNNUM + prn; break;
    }

    return sat;
}