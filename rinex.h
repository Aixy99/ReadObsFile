#ifndef RINEX_H
#define RINEX_H
#include <stdio.h>
#include <string.h>

typedef unsigned int uint;
typedef enum {			//����ϵͳ
	G = 0,	//GPS		ȫ��λϵͳ(����)			
	R = 1,	//GLONASS	������˹(����/����˹)	
	E = 2,	//Galileo	٤����(ŷ��)				
	J = 3,	//QZSS		׼�춥����ϵͳ(�ձ���	
	S = 4,	//SBAS		�ǻ���ǿϵͳ			
	C = 5,	//BDS		����(�й�)				
	I = 6,	//IRNSS		ӡ�����򵼺�����ϵͳ(ӡ��)	
	SYSNUM	//ϵͳ����
};
	
typedef enum {			//�۲�����
	_C = 0,	//α��
	_L = 1,	//�ز���λ
	_D = 2,	//������
	_S = 3	//�����
};


//��������
#define MAXCHANNELNUM 7							//���ͨ������
#define MAXOBSTYPE 64							//���۲���������
#define MAXGPSNUM 128							//GPS�����������
#define MAXGLONUM 128							//GLO�����������
#define MAXGALNUM 128							//GAL�����������
#define MAXQZSNUM 128							//QZS�����������
#define MAXSBSNUM 128							//SBS�����������
#define MAXBDSNUM 128							//BDS�����������
#define MAXIRNNUM 128							//IRN�����������
											
//�ṹ�嶨��
typedef struct {	// ʱ��ṹ
	time_t time;    // �ɱ�׼time_t��ʾ��ʱ�� (s)
	double sec;     // 1�����µ�С������
} gtime_t;

typedef struct {	//�۲����ݼ�¼
	gtime_t time;	//��ǰ��Ԫ
	uint sat;		//���Ǳ��
	uint SNR[MAXCHANNELNUM];	//�����
	double P[MAXCHANNELNUM];	//α��
	double L[MAXCHANNELNUM];	//�ز���λ
	double D[MAXCHANNELNUM];	//������Ƶ��
} obsd_t;

typedef struct {	//�۲�����
	uint n;				//��Ԫ����
	obsd_t data[2880];	//����
} obs_t;

//��������
extern void readObsFileH(FILE* file, char* type, char* buff, char tobs[][MAXOBSTYPE][4]);
extern void readObsFileB(FILE* file, char* buff, const char tobs[][MAXOBSTYPE][4], int ind[7][36]);
extern void readObsFile(char* file);
#endif  // !RINEX_H
