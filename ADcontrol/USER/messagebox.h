#ifndef _messagebox_h
#define _messagebox_h

#include "stm32f4xx.h"

#define FLASH_SAVE_ADDR  0X0800C004 

//Semaphores �ĳ�Ա     //������ź���

#define Key_Set     0
#define Key_Up      Key_Set+1
#define Key_Down    Key_Up+1
#define Key_Return  Key_Down+1
#define _1s_flag    Key_Return+1
#define _4s_flag    _1s_flag+1
#define WorkState   _4s_flag+1
#define Key_Down_Long WorkState+1
#define Key_Up_Long Key_Down_Long+1
#define Total       Key_Up_Long+1

//StateList �ĳ�Ա      //״̬
#define NeedDehumidity  0
#define AverageTemp     NeedDehumidity+1
#define AverageHumi     AverageTemp+1
#define OutsideTemp     AverageHumi+1
#define OutsideHumi     OutsideTemp+1
#define RoomTemp        OutsideHumi+1
#define RoomHumi        RoomTemp+1
#define NeedADCool      RoomHumi+1
#define NeedXinfengCool NeedADCool+1
#define NeedWash        NeedXinfengCool+1
#define Xinfengqueshui  NeedWash+1
#define Xinfengtongfeng Xinfengqueshui+1
#define Xinfengshuileng Xinfengtongfeng+1
#define Xinfengqingxi   Xinfengshuileng+1
#define PowerOff        Xinfengqingxi+1
#define Total1    PowerOff+1

//SetData �ĳ�Ա        
#define SetTemp      0
#define LimitHumidity   SetTemp+1        //�趨ʪ��
#define LimitTemp       LimitHumidity+1     //�趨�¶�
#define WashingInterval LimitTemp+1     //��ϴ����
#define MyAddress       WashingInterval+1 //������ַ
#define BaudRate1       MyAddress+1
#define BaudRate2       BaudRate1+1
#define BaudRate3       BaudRate2+1
#define WorkDays        BaudRate3+1
#define Total2    WorkDays+1


//UpdataFlag�ĳ�Ա
#define DisplayUpdata 0
#define TempUpdata  DisplayUpdata+1
#define UsartUpdata TempUpdata+1
#define Total3  UsartUpdata+1


#define XFSLControlErr 0
#define ADControlErr XFSLControlErr+1
#define ShortOfWater    ADControlErr+1
#define LowEfficiency   ShortOfWater+1
#define GetTempErr      LowEfficiency+1
#define DeHumiErr       GetTempErr+1
#define NoAD       DeHumiErr+1
#define Total4  NoAD+1

extern s32 Semaphores[Total];
extern s32 StateList[Total1];
extern s32 SetData[Total2];
extern s32 UpdataFlag[Total3];
extern s32 ErrorList[Total4];

typedef struct {
    char EN;
    short  tempInside;
    short  tempOutside;
    short  settingTemp;
    short  settingHumi;
    short  humi;
    u8 State;
    u8 warning[3];
    
}_AirConditionList;


extern _AirConditionList  ADList[256];
void Semaphores_Init(void);
void Ifnodata(void);
void yuejie(void);

#endif


