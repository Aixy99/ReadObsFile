#ifndef RINEX_H
#define RINEX_H
#include <stdio.h>
#include <string.h>

typedef unsigned int uint;
typedef enum {			//卫星系统
	G = 0,	//GPS		全球定位系统(美国)			
	R = 1,	//GLONASS	格洛纳斯(苏联/俄罗斯)	
	E = 2,	//Galileo	伽利略(欧盟)				
	J = 3,	//QZSS		准天顶卫星系统(日本）	
	S = 4,	//SBAS		星基增强系统			
	C = 5,	//BDS		北斗(中国)				
	I = 6,	//IRNSS		印度区域导航卫星系统(印度)	
	SYSNUM	//系统个数
};
	
typedef enum {			//观测类型
	_C = 0,	//伪距
	_L = 1,	//载波相位
	_D = 2,	//多普勒
	_S = 3	//信噪比
};


//常量定义
#define MAXCHANNELNUM 7							//最大通道数量
#define MAXOBSTYPE 64							//最大观测类型数量
#define MAXGPSNUM 128							//GPS卫星最大数量
#define MAXGLONUM 128							//GLO卫星最大数量
#define MAXGALNUM 128							//GAL卫星最大数量
#define MAXQZSNUM 128							//QZS卫星最大数量
#define MAXSBSNUM 128							//SBS卫星最大数量
#define MAXBDSNUM 128							//BDS卫星最大数量
#define MAXIRNNUM 128							//IRN卫星最大数量
											
//结构体定义
typedef struct {	// 时间结构
	time_t time;    // 由标准time_t表示的时间 (s)
	double sec;     // 1秒以下的小数部分
} gtime_t;

typedef struct {	//观测数据记录
	gtime_t time;	//当前历元
	uint sat;		//卫星编号
	uint SNR[MAXCHANNELNUM];	//信噪比
	double P[MAXCHANNELNUM];	//伪距
	double L[MAXCHANNELNUM];	//载波相位
	double D[MAXCHANNELNUM];	//多普勒频率
} obsd_t;

typedef struct {	//观测数据
	uint n;				//历元个数
	obsd_t data[2880];	//数据
} obs_t;

//函数声明
extern void readObsFileH(FILE* file, char* type, char* buff, char tobs[][MAXOBSTYPE][4]);
extern void readObsFileB(FILE* file, char* buff, const char tobs[][MAXOBSTYPE][4], int ind[7][36]);
extern void readObsFile(char* file);
#endif  // !RINEX_H
