#ifndef _CLOCK_H
#define _CLOCK_H
#include "stm32f4xx.h"


#define YY	0
#define MM	1
#define DD	2
#define hh	3
#define mm	4
#define ss	5

extern s8 Time_String_YMD[];   //��������յ��ַ���
extern s8 Time_String_HMS[];   //���ʱ������ַ���

extern s8 NowTime[6];          //���ʱ�������
extern s8 SetTime[6];

void Clock_Updata(void);        //ʱ�ӡ�
void ClockToString(void);       //��ʱ������ת�����ַ���
void ReadTimeFromChip(void);    //��ʱ��оƬ����ʱ��
void CopyNowToSet(void);
void SaveSetTime(void);
void shijianyuejie1(s8* time);
void shijianyuejie2(s8* time);

#endif

