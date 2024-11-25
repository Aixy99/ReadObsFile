#include "rinex.h"
#include "utility.h"

const char syscodes[] = "GREJSCI";				//卫星系统代码
const char obscodes[] = "CLDS";					//观测类型码

/*读取O文件头*/
extern void readObsFileH(FILE *file, char *type, char *buff, char tobs[][MAXOBSTYPE][4]) {
	printf("readObsFileH()\n");//调试

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
		} else if (strstr(buff, "END OF HEADER")) {
			break;
		}
	}
}

/*读取o文件体*/
extern void readObsFileB() {
	printf("readObsFileB()\n");//调试
}

/*读取o文件*/
extern void readObsFile(char *file) {
	printf("readObsFile()\n");//调试

	FILE* fp;						//文件指针
	char type;						//文件类型
	char buff[1024];				//用于存储当前行
	char tobs[7][MAXOBSTYPE][4] = { {0} };	//观测类型

	fp = fopen(file, "r");
	if (fp != NULL) {
		printf("文件打开成功\n");
	} else {
		printf("文件打开失败\n");
		return;
	}

	//读取文件头
	readObsFileH(fp, &type, buff, tobs);
	//读取文件体
	readObsFileB();
}