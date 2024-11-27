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

/*��ʼ��obs*/
static void initObs(obs_t* obs, int* satSum) {
	memset(obs, 0, sizeof(obs_t));
	obs->data = malloc((2800 * (*satSum) * sizeof(obsd_t)));
	memset(obs->data, 0, sizeof(obsd_t));
}

/*��������*/
static int decode_ind(const int tInd, const int ind[7][36], int *type, int *channel, char satCode) {
	int sysInd;		//ϵͳ����
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

/*��ȡO�ļ�ͷ*/
extern void readObsFileH(FILE *file, char *type, char *buff, char tobs[][MAXOBSTYPE][4], int *satSum) {
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
		} else if (strstr(buff,"# OF SATELLITES")) {
			*satSum = str2num(buff, 0, 6);
		} else if (strstr(buff, "END OF HEADER")) {
			break;
		}
	}
}

/*��ȡo�ļ���*/
extern void readObsFileB(FILE* file, char* buff, const char tobs[][MAXOBSTYPE][4], int ind[7][36], obs_t *o, int* satSum) {
	obs_t obs;
	int ns = 0;			//��ǰ��Ԫ��������
	int satNum;			//���Ǳ��
	gtime_t time = { 0 };	//��Ԫ
	setInd(tobs, ind);	//��������
	static int i = 0;	//��ǰ��������
	int n = 0;			//��ǰ��Ԫ����
	double val = 0;		//������ʱ����ֵ

	initObs(&obs, satSum);
	while (fgets(buff, 1024, file)) {
		if (strstr(buff, "> ")) {
			str2time(buff, 2, 27, &time);
			ns = str2num(buff, 32, 3);
		} else {
			char satCode[4];	//���Ǵ���
			setstr(satCode, buff, 3);
			satNum = setSatNum(satCode);
			obs.data[i].sat = satNum;	//��¼���Ǳ��
			obs.data[i].time = time;	//��¼ʱ��

			int type;		//��������
			int channel;	//ͨ��

			int tInd = 0;	//��������
			while (decode_ind(tInd, ind, &type, &channel, satCode[0])) {
				val = str2num(buff, 3 + 16 * tInd++, 14);	//��ȡ��ǰֵ

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


/*��ȡo�ļ�*/
extern void readObsFile(char *file) {
	obs_t obs;						//�۲�����
	FILE* fp;						//�ļ�ָ��
	char type;						//�ļ�����
	char buff[1024];				//���ڴ洢��ǰ��
	char tobs[SYSNUM][MAXOBSTYPE][4] = { {0} };	//�۲�����
	int ind[SYSNUM][36] = { {0} };		//�۲���������
	int satSum = 0;					//�й۲�ֵ����������
	fp = fopen(file, "r");
	if (fp != NULL) {
		printf("�ļ��򿪳ɹ�\n");
	} else {
		printf("�ļ���ʧ��\n");
		return;
	}

	//��ȡ�ļ�ͷ
	readObsFileH(fp, &type, buff, tobs, &satSum);
	//��ȡ�ļ���
	readObsFileB(fp, buff, tobs, ind, &obs, &satSum);
}