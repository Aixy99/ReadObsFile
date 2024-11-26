#include "rinex.h"
#include "utility.h"

const char syscodes[] = "GREJSCI";				//����ϵͳ����
const char obscodes[] = "CLDS";					//�۲�������
/*��������
* �����Ľṹ����:
*	0	|	01	02	03	04	11	12	14	23	24	33
* ��ǰ��� 0 ��������ϵͳ�Ĵ���,�� 0 : GPS
*		0Ҳ�Ƕ�ά������к�
* ��������־�����λ���
*		���е�һλ����۲�����(CLDS),
*		�ڶ�λΪ�۲��Ƶ��
*		����: 01��ʾ C1_
*/
static void setInd(const char tobs[SYSNUM][MAXOBSTYPE][4], int ind[SYSNUM][36]) {
	char type;		//�۲���������(CLDS)
	int channel = 0;	//Ƶ��
	for(int i = 0; i < SYSNUM; i++){	//����ÿ��ϵͳ
		for (int j = 0; ; j++) {		//����ÿ���۲�����
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

/*��ȡO�ļ�ͷ*/
extern void readObsFileH(FILE *file, char *type, char *buff, char tobs[][MAXOBSTYPE][4]) {
	printf("readObsFileH()\n");//����

	while (fgets(buff, 1024, file)) {
		if (strstr(buff, "RINEX VERSION / TYPE")) {
			*type = *(buff + 20);
		} else if (strstr(buff, "SYS / # / OBS TYPES")) {
			
			char *p = strchr(syscodes, buff[0]);		//ָ��pָ��ǰ�е�����ϵͳ
			int i = (int)(p - syscodes);				//����ϵͳ���
			int n = str2num(buff, 3, 3);				//�۲���������
			int nt = 0;									//�۲����ͼ���
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

/*��ȡo�ļ���*/
extern void readObsFileB(FILE* file, char* buff, const char tobs[][MAXOBSTYPE][4], int ind[7][36], obs_t *obs) {
	printf("readObsFileB()\n");//����

	int ns = 0;			//��������
	int satNum;			//���Ǳ��
	gtime_t time = { 0 };	//��Ԫ
	setInd(tobs, ind);	//��������
	int i = 0;			//��ǰ��������
	int n = 0;			//��ǰ��Ԫ����

	while (fgets(buff, 1024, file)) {
		if (strstr(buff, "> ")) {
			str2time(buff, 2, 27, &time);
			ns = str2num(buff, 32, 3);
			i = 0;
		} else {
			for (; i < ns; i++) {
				char satCode[4];	//���Ǵ���
				setstr(satCode, buff, 3);
				satNum = setSatNum(satCode);
			}
		}
	}
}

/*��ȡo�ļ�*/
extern void readObsFile(char *file) {
	printf("readObsFile()\n");//����

	obs_t obs;						//�۲�����
	FILE* fp;						//�ļ�ָ��
	char type;						//�ļ�����
	char buff[1024];				//���ڴ洢��ǰ��
	char tobs[SYSNUM][MAXOBSTYPE][4] = { {0} };	//�۲�����
	int ind[SYSNUM][36] = { {0} };		//�۲���������

	fp = fopen(file, "r");
	if (fp != NULL) {
		printf("�ļ��򿪳ɹ�\n");
	} else {
		printf("�ļ���ʧ��\n");
		return;
	}

	//��ȡ�ļ�ͷ
	readObsFileH(fp, &type, buff, tobs);
	//��ȡ�ļ���
	readObsFileB(fp, buff, tobs, ind, &obs);
}