#include "rinex.h"
#include "utility.h"

const char syscodes[] = "GREJSCI";				//卫星系统代码
const char obscodes[] = "CLDS";					//观测类型码
/*设置索引
* 索引的结构如下:
*	0	|	01	02	03	04	11	12	14	23	24	33
* 最前面的 0 代表卫星系统的代号,如 0 : GPS
*		0也是二维数组的行号
* 后面的数字均由两位组成
*		其中第一位代表观测类型(CLDS),
*		第二位为观测的频道
*		例如: 01表示 C1_
*/
static void setInd(const char tobs[SYSNUM][MAXOBSTYPE][4], int ind[SYSNUM][36]) {
	char type;		//观测数据类型(CLDS)
	int channel = 0;	//频道
	for(int i = 0; i < SYSNUM; i++){	//遍历每个系统
		for (int j = 0; ; j++) {		//遍历每个观测类型
			if (tobs[i][j][0] == 0) break;
			type = tobs[i][j][0];
			channel = str2num(tobs[i][j], 1, 1);

			switch (type) {
				case 'C':	type = _C; break;
				case 'L':	type = _L; break;
				case 'D':	type = _D; break;
				case 'S':	type = _S; break;
			}
			ind[i][j] = type * 10 + channel;
		}
	}
}

/*初始化obs*/
static void initObs(obs_t* obs, int* satSum) {
	memset(obs, 0, sizeof(obs_t));
	obs->data = malloc((2800 * (*satSum) * sizeof(obsd_t)));
	memset(obs->data, 0, sizeof(obsd_t));
}

/*解码索引*/
static int decode_ind(const int tInd, const int ind[7][36], int *type, int *channel, char satCode) {
	int sysInd;		//系统索引
	switch (satCode) {
		case 'G':	sysInd = G; break;
		case 'R':	sysInd = R; break;
		case 'E':	sysInd = E; break;
		case 'J':	sysInd = J; break;
		case 'S':	sysInd = S; break;
		case 'C':	sysInd = C; break;
		case 'I':	sysInd = I; break;
	}

	if (ind[sysInd][tInd] == 0) return 0;

	*type = ind[sysInd][tInd] / 10;
	const int frqcode[7] = { 1,2,5,6,7,8,9 };
	for (int i = 0; i < 7; i++) {
		if (ind[sysInd][tInd] % 10 == frqcode[i]) {
			*channel = i;
			break;
		}
	}

	return 1;
}

/*读取O文件头*/
extern void readObsFileH(FILE *file, char *type, char *buff, char tobs[][MAXOBSTYPE][4], int *satSum) {
	while (fgets(buff, 1024, file)) {
		if (strstr(buff, "RINEX VERSION / TYPE")) {
			*type = *(buff + 20);
		} else if (strstr(buff, "SYS / # / OBS TYPES")) {
			
			char *p = strchr(syscodes, buff[0]);		//指针p指向当前行的卫星系统
			int i = (int)(p - syscodes);				//卫星系统编号
			int n = str2num(buff, 3, 3);				//观测类型数量
			int nt = 0;									//观测类型计数
			for (int j = 0, k = 7; j < n; j++, k += 4) {
				if (k > 58) {
					if (!fgets(buff, 1024, file)) break;
					k = 7;
				}
				if (nt < MAXOBSTYPE - 1) {
					setstr(tobs[i][nt++], buff + k, 3);
				}
			}
			*tobs[i][nt] = 0;
		} else if (strstr(buff,"# OF SATELLITES")) {
			*satSum = str2num(buff, 0, 6);
		} else if (strstr(buff, "END OF HEADER")) {
			break;
		}
	}
}

/*读取o文件体*/
extern void readObsFileB(FILE* file, char* buff, const char tobs[][MAXOBSTYPE][4], int ind[7][36], obs_t *o, int* satSum) {
	obs_t obs;
	int ns = 0;			//当前历元卫星数量
	int satNum;			//卫星编号
	gtime_t time = { 0 };	//历元
	setInd(tobs, ind);	//设置索引
	static int i = 0;	//当前保存序数
	int n = 0;			//当前历元序数
	double val = 0;		//用来临时保存值

	initObs(&obs, satSum);
	while (fgets(buff, 1024, file)) {
		if (strstr(buff, "> ")) {
			str2time(buff, 2, 27, &time);
			ns = str2num(buff, 32, 3);
		} else {
			char satCode[4];	//卫星代码
			setstr(satCode, buff, 3);
			satNum = setSatNum(satCode);
			obs.data[i].sat = satNum;	//记录卫星编号
			obs.data[i].time = time;	//记录时间

			int type;		//数据类型
			int channel;	//通道

			int tInd = 0;	//类型索引
			while (decode_ind(tInd, ind, &type, &channel, satCode[0])) {
				val = str2num(buff, 3 + 16 * tInd++, 14);	//读取当前值

				if (type == _C) {
					obs.data[i].P[channel] = val;
				} else if (type == _L) {
					obs.data[i].L[channel] = val;
				} else if (type == _D) {
					obs.data[i].D[channel] = val;
				} else if (type == _S) {
					obs.data[i].SNR[channel] = val;
				}
			}
			i++;
		}
	}

	o->data = obs.data;
	o->n = i + 1;
}


/*读取o文件*/
extern void readObsFile(char *file) {
	obs_t obs;						//观测数据
	FILE* fp;						//文件指针
	char type;						//文件类型
	char buff[1024];				//用于存储当前行
	char tobs[SYSNUM][MAXOBSTYPE][4] = { {0} };	//观测类型
	int ind[SYSNUM][36] = { {0} };		//观测类型索引
	int satSum = 0;					//有观测值的卫星数量
	fp = fopen(file, "r");
	if (fp != NULL) {
		printf("文件打开成功\n");
	} else {
		printf("文件打开失败\n");
		return;
	}

	//读取文件头
	readObsFileH(fp, &type, buff, tobs, &satSum);
	//读取文件体
	readObsFileB(fp, buff, tobs, ind, &obs, &satSum);
}