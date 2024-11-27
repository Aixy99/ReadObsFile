#include "rinex.h"
#include "utility.h"

int main() { 
	char* file = "C:/Users/w2151/Documents/C project/data/savo0160.22o";
	obs_t obs;
	readObsFile(file, &obs);

	char satCode[4] = "C20";	//卫星代码
	char typeCode[2] = "S";		//类型代码

	int satNum;			//要输出的卫星编号
	int channel = 2;	//要输出的通道
	int type;			//要输出的类型
	double val;			//要输出的值

	//printf("请输入你要输出的卫星代码(例如,G01):");
	//scanf("%s", satCode);
	//printf("请输入你要输出的通道(1256789):");
	//scanf("%d", &channel);
	//printf("请输入你要输出的数据类型(CLDS):");
	//scanf("%s", typeCode);
	
	satNum = setSatNum(satCode);				//解析卫星编号

	const int frqcode[7] = { 1,2,5,6,7,8,9 };	//解析通道
	for (int i = 0; i < 7; i++) {
		if (channel == frqcode[i]) {
			channel = i;
			break;
		}
	}

	switch (typeCode[0]) {						//解析数据类型
		case 'C':	type = _C; break;
		case 'L':	type = _L; break;
		case 'D':	type = _D; break;
		case 'S':	type = _S; break;
	}

	for (int i = 0; i < obs.n; i++) {			//输出
		if (obs.data[i].sat != satNum) continue;
		switch (type) {
			case _C:	val = obs.data[i].P[channel]; break;
			case _L:	val = obs.data[i].L[channel]; break;
			case _D:	val = obs.data[i].D[channel]; break;
			case _S:	val = obs.data[i].SNR[channel]; break;
		}
		printf("%lf\t\t%lf\n", obs.data[i].time.time + obs.data[i].time.sec, val);
	}
	return 0; 
}