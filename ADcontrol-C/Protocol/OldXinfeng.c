#include "led.h"
#include "messagebox.h"
#include "OldXinfeng.h"

#define OUT_KAIGUAN     GPIOE,GPIO_Pin_10
#define OUT_TONGFENG    GPIOE,GPIO_Pin_11
#define OUT_SHUILENG    GPIOE,GPIO_Pin_12
#define OUT_QINGXI      GPIOE,GPIO_Pin_13

#define OUT_UNDEFINE1   GPIOC,GPIO_Pin_7
#define OUT_UNDEFINE2   GPIOC,GPIO_Pin_8

void Xinfeng_Init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;//| GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIO_SetBits(GPIOE, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
//	GPIO_SetBits(GPIOC,  GPIO_Pin_7 ); //所在的继电器有问题
    GPIO_SetBits(GPIOC,  GPIO_Pin_8);
}

//触发开关按钮
void Xinfeng_Kaiguan(u8 en)
{
    static u16 cnt=500;
    if(en == 1)
    {
        cnt = 0;
    }
    if(cnt == 0)
    {
        GPIO_ResetBits(OUT_KAIGUAN);
        cnt++;
    }
    else if(cnt == 500)
    {
        GPIO_SetBits(OUT_KAIGUAN); 
    }
    else if(cnt < 500)
    {
        cnt++;
    }
}

//触发通风按钮
void Xinfeng_Tongfeng(u8 en)
{
    static u16 cnt=500;
    if(en == 1)
    {
        cnt = 0;
    }
    if(cnt == 0)
    {
        GPIO_ResetBits(OUT_TONGFENG);
        cnt++;
    }
    else if(cnt == 500)
    {
        GPIO_SetBits(OUT_TONGFENG);

    }
    else if(cnt < 500)
    {
        cnt++;
    }
}

//触发水冷按钮
void Xinfeng_Shuileng(u8 en)
{
    static u16 cnt=500;
    if(en == 1)
    {
        cnt = 0;
    }
    if(cnt == 0)
    {
        GPIO_ResetBits(OUT_SHUILENG);
        cnt++;
    }
    else if(cnt == 500)
    {
        GPIO_SetBits(OUT_SHUILENG);

    }
    else if(cnt < 500)
    {
        cnt++;
    }
}

//触发清洗按钮
void Xinfeng_Qingxi(u8 en)
{
    static u16 cnt=500;
    if(en == 1)
    {
        
        cnt = 0;
    }
    if(cnt == 0)
    {
        GPIO_ResetBits(OUT_QINGXI);
        cnt++;
    }
    else if(cnt == 500)
    {
        GPIO_SetBits(OUT_QINGXI);

    }
    else if(cnt < 500)
    {
        cnt++;
    }
}

void XinfengStateRead()
{
    if(Key_Read(KEY_QUESHUI))
    {
        StateList[Xinfengqueshui] = 0;
        ErrorList[ShortOfWater] = 0;
        GPIO_SetBits(LED2);
    }
    else
    {
        ErrorList[ShortOfWater] = 1;
        StateList[Xinfengqueshui] = 1;
        GPIO_ResetBits(LED2);
    }
    
    if(Key_Read(KEY_TONGFENG))
    {
        StateList[Xinfengtongfeng] = 0;

    }
    else
    {
        StateList[Xinfengtongfeng] = 1;

    }
        
    if(Key_Read(KEY_SHUILENG))
    {
        StateList[Xinfengshuileng] = 0;

    }
    else
    {
        StateList[Xinfengshuileng] = 1;
        GPIO_ResetBits(LED1);
    }
        
    if(Key_Read(KEY_QINGXI))
    {
        StateList[Xinfengqingxi] = 0;
        GPIO_SetBits(LED5);
    }
    else
    {
        SetData[WorkDays] = 0;
        StateList[Xinfengqingxi] = 1;
        GPIO_ResetBits(LED5);
    }
    if(StateList[Xinfengshuileng] || StateList[Xinfengtongfeng])
        GPIO_ResetBits(LED1);
    else 
        GPIO_SetBits(LED1);
}


