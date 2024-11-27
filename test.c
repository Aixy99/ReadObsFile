#include "rinex.h"
#include "utility.h"

int main() { 
	char* file = "C:/Users/w2151/Documents/C project/data/savo0160.22o";
	obs_t obs;
	readObsFile(file, &obs);

	char satCode[4] = "C20";	//���Ǵ���
	char typeCode[2] = "S";		//���ʹ���

	int satNum;			//Ҫ��������Ǳ��
	int channel = 2;	//Ҫ�����ͨ��
	int type;			//Ҫ���������
	double val;			//Ҫ�����ֵ

	//printf("��������Ҫ��������Ǵ���(����,G01):");
	//scanf("%s", satCode);
	//printf("��������Ҫ�����ͨ��(1256789):");
	//scanf("%d", &channel);
	//printf("��������Ҫ�������������(CLDS):");
	//scanf("%s", typeCode);
	
	satNum = setSatNum(satCode);				//�������Ǳ��

	const int frqcode[7] = { 1,2,5,6,7,8,9 };	//����ͨ��
	for (int i = 0; i < 7; i++) {
		if (channel == frqcode[i]) {
			channel = i;
			break;
		}
	}

	switch (typeCode[0]) {						//������������
		case 'C':	type = _C; break;
		case 'L':	type = _L; break;
		case 'D':	type = _D; break;
		case 'S':	type = _S; break;
	}

	for (int i = 0; i < obs.n; i++) {			//���
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