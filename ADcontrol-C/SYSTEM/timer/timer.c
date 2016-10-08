#include "timer.h"
#include "stm32f4xx_tim.h"
#include "messagebox.h"
#include "led.h"
#include "Clock.h"
#include "OldXinfeng.h"
#include "pre_send.h"
#define KEYLONGTIME 75

u8 _1msflag = 0;
u8 _2sflag = 0;
u8 _5sflag = 0;
u8 _1sflag = 0;
int tempUpdataCnt=0;
void TaskTimerInit()
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInitStructure.TIM_Prescaler = 83;
    TIM_TimeBaseInitStructure.TIM_Period = 999;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    
    TIM_Cmd(TIM2,ENABLE); 
	
		NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; 
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
		NVIC_Init(&NVIC_InitStructure);
}
unsigned int xinfeng15s = 1;
void TIM2_IRQHandler(void) //1ms
{
    static int clockcnt;
    static int keydelay;
    static int temp_cnt;
    static int keyuplongcnt;
    static int keydownlongcnt;
    static int _1scnt;
    static int _2scnt;
    static int _5scnt;
    static int _15scnt;
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
            _1msflag++;
			_1scnt++;
            if(_1scnt >=1000)
            {
                _1sflag = 1;
                tempUpdataCnt++;
                _1scnt = 0;
            }
            _2scnt++;
            if(_2scnt >=2000)
            {
                _2sflag = 1;
                _2scnt = 0;
            }
            _5scnt++;
            if(_5scnt >=5000)
            {
                _5sflag = 1;
                _15scnt++;
                _5scnt = 0;
            }
            if(_15scnt>=3)
            {
                xinfeng15s = 1;
                _15scnt = 0;
            }
            
            if(CMDBusy == 1)
            {
                TimeOutCnt++;
                if(TimeOutCnt >=100)
                {
                    CMDState = TIMEOUT;
                    CMDBusy = 0;
                    ADList[NowADDR].EN = 0;
                    NowADDR = 0;
                    TimeOutCnt = 0;
                }
            }
            else TimeOutCnt = 0;
            clockcnt++;
            if(clockcnt>=1000)
            {
                clockcnt=0;
                Clock_Updata();
            }
			keydelay++;
			if(keydelay >=20)
			{
					keydelay = 0;
					if(Semaphores[Key_Set] == 0)
					{
							if(!Key_Read(KEY_SET))
							{
									Semaphores[Key_Set] = 2;
							}
					}
					else if(Semaphores[Key_Set] == 2)
					{
							if(Key_Read(KEY_SET))
							{
									Semaphores[Key_Set] = 1;                   
							}
					}
					
					if(Semaphores[Key_Up] == 0)
					{
							if(Key_Read(KEY_UP) == 0)
									Semaphores[Key_Up] = 2;
					}
					else if(Semaphores[Key_Up] == 2)
					{
                            keyuplongcnt++;
                            if(keyuplongcnt>=KEYLONGTIME)
                            {
                                if(keyuplongcnt%20==0)
                                    Semaphores[Key_Up_Long] = 1;
                            }
							if(Key_Read(KEY_UP) && keyuplongcnt<KEYLONGTIME)
							{
                                keyuplongcnt = 0;
								Semaphores[Key_Up] = 1;
							}
                            else if(Key_Read(KEY_UP) && keyuplongcnt>=KEYLONGTIME)
                            {
                                keyuplongcnt = 0;
                                Semaphores[Key_Up_Long] = 0;
                            }
					}
					
					if(Semaphores[Key_Down] == 0)
					{
							if(Key_Read(KEY_DOWN) == 0)
									Semaphores[Key_Down] = 2;
					}
					else if(Semaphores[Key_Down] == 2)
					{
                            keydownlongcnt++;
                            if(keydownlongcnt>=KEYLONGTIME)
                            {
                                if(keydownlongcnt%20==0)
                                    Semaphores[Key_Down_Long] = 1;
                            }
							if(Key_Read(KEY_DOWN) && keydownlongcnt<KEYLONGTIME)
							{
                                keydownlongcnt = 0;
								Semaphores[Key_Down] = 1;
							}
                            else if(Key_Read(KEY_DOWN) && keydownlongcnt>=KEYLONGTIME)
                            {
                                keydownlongcnt = 0;
                                Semaphores[Key_Down_Long] = 0;
                            }
					}
					
					if(Semaphores[Key_Return] == 0)
					{
							if(Key_Read(KEY_RETURN) == 0)
									Semaphores[Key_Return] = 2;
					}
					else if(Semaphores[Key_Return] == 2)
					{
							if(Key_Read(KEY_RETURN))
									Semaphores[Key_Return] = 1;
					}
			}//按键检测
            
            Xinfeng_Kaiguan(0);
            Xinfeng_Tongfeng(0);
            Xinfeng_Shuileng(0);
            Xinfeng_Qingxi(0);

            temp_cnt++;
            if(temp_cnt>=5000)
            {
                UpdataFlag[TempUpdata] = 1;
            }
            

	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

