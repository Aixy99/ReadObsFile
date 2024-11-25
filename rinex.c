#include "rinex.h"
#include "utility.h"

const char syscodes[] = "GREJSCI";				//����ϵͳ����
const char obscodes[] = "CLDS";					//�۲�������

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
extern void readObsFileB() {
	printf("readObsFileB()\n");//����
}

/*��ȡo�ļ�*/
extern void readObsFile(char *file) {
	printf("readObsFile()\n");//����

	FILE* fp;						//�ļ�ָ��
	char type;						//�ļ�����
	char buff[1024];				//���ڴ洢��ǰ��
	char tobs[7][MAXOBSTYPE][4] = { {0} };	//�۲�����

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
	readObsFileB();
}