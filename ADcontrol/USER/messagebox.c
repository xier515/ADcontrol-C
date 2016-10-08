#include "messagebox.h"
#include "stmflash.h"

s32 Semaphores[Total]; //集中存放信号量
s32 StateList[Total1]; //集中存放状态量
s32 SetData[Total2];   //存放需要保存的设置信息。
s32 UpdataFlag[Total3];
s32 ErrorList[Total4];

_AirConditionList ADList[256];

void Ifnodata(void)
{
    if(!~SetData[SetTemp])
        SetData[SetTemp] = 25;
    if(!~SetData[LimitHumidity])
        SetData[LimitHumidity] = 80;
    if(!~SetData[LimitTemp])
        SetData[LimitTemp] = 5;
    if(!~SetData[WashingInterval])
        SetData[WashingInterval] = 30;
    if(!~SetData[MyAddress])
        SetData[MyAddress] = 1;    
    if(!~SetData[BaudRate1])
        SetData[BaudRate1] = 5;
    if(!~SetData[BaudRate2])
        SetData[BaudRate2] = 5;
    if(!~SetData[BaudRate3])
        SetData[BaudRate3] = 5;
    if(!~SetData[WorkDays])
        SetData[WorkDays] = 0;
    if(!StateList[OutsideTemp])
        StateList[OutsideTemp] = 17;
}

void Semaphores_Init(void)
{
	u16 i;
	for (i = 0; i < Total; i++)
	{
		Semaphores[i] = 0;
	}
    for (i = 0; i < Total1; i++)
	{
		StateList[i] = 0;
	}
    for (i = 0; i < Total2; i++)
	{
		SetData[i] = 0;
	}
    for(i = 0; i < 256; i++)
    {
        (ADList+i)->EN = 0;
        (ADList+i)->tempInside = 0x7fff;
        (ADList+i)->tempOutside = 0x7fff;
        (ADList+i)->humi = 0x7fff;
        (ADList+i)->State = 0;
        (ADList+i)->warning[0] = 0;
        (ADList+i)->warning[1] = 0;
        (ADList+i)->warning[2] = 0;
    }
		STMFLASH_Read(FLASH_SAVE_ADDR, (u32*)SetData, sizeof(SetData)/4);
    Ifnodata();
}


void yuejie(void)
{
    if(SetData[SetTemp]<18)
        SetData[SetTemp] = 18;
    else if(SetData[SetTemp] >30)
        SetData[SetTemp] = 30;
    
    if(SetData[LimitHumidity]<60)
        SetData[LimitHumidity] = 60;
    else if(SetData[LimitHumidity] >100)
        SetData[LimitHumidity] = 100;
    
    if(SetData[LimitTemp]<3)
        SetData[LimitTemp] = 3;
    else if(SetData[LimitTemp] >15)
        SetData[LimitTemp] = 15;

    if(SetData[WashingInterval]<30)
        SetData[WashingInterval] = 30;
    else if(SetData[WashingInterval] >100)
        SetData[WashingInterval] = 100;    
        
    if(SetData[MyAddress]<1)
        SetData[MyAddress] = 1;
    else if(SetData[MyAddress] >100)
        SetData[MyAddress] = 100;    

    if(SetData[BaudRate1]<1)
        SetData[BaudRate1] = 5;
    else if(SetData[BaudRate1] >5)
        SetData[BaudRate1] = 1; 
    
    if(SetData[BaudRate2]<1)
        SetData[BaudRate2] = 5;
    else if(SetData[BaudRate2] >5)
        SetData[BaudRate2] = 1; 
    
    if(SetData[BaudRate3]<1)
        SetData[BaudRate3] = 5;
    else if(SetData[BaudRate3] >5)
        SetData[BaudRate3] = 1 ; 
}


