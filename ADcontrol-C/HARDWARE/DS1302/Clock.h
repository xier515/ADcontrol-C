#ifndef _CLOCK_H
#define _CLOCK_H
#include "stm32f4xx.h"


#define YY	0
#define MM	1
#define DD	2
#define hh	3
#define mm	4
#define ss	5

extern s8 Time_String_YMD[];   //存放年月日的字符串
extern s8 Time_String_HMS[];   //存放时分秒的字符串

extern s8 NowTime[6];          //存放时间的数组
extern s8 SetTime[6];

void Clock_Updata(void);        //时钟。
void ClockToString(void);       //把时间数组转换成字符串
void ReadTimeFromChip(void);    //从时钟芯片读出时间
void CopyNowToSet(void);
void SaveSetTime(void);
void shijianyuejie1(s8* time);
void shijianyuejie2(s8* time);

#endif

