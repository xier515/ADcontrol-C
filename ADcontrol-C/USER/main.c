#include "delay.h"
#include "RS485.h"
#include "led.h"
#include "string.h"
#include "12864.h"
#include "timer.h"
#include "Show.h"
#include "messagebox.h"
#include "OldXinfeng.h"
#include "iwdog.h"
#include "ds1302.h"
#include "Clock.h"
#include "DME3000.h"
#include "i2c_hal.h"
#include "shtc1.h"
#include "logic.h"
#include "action.h"
#include "stmflash.h"
#include "debug.h"


unsigned int RunTime=0;

int main(void)
{ 
    u32 i=0,errtest=0,washingtime=0,searchtime=0;
    My_IWDGInit();
	delay_init(168);	
    Semaphores_Init();  
	MyLED_Init();
    MyKey_Init();
    SHTC1_Init();
    DS1302_Pin_Init();
    Time_Updata0();
    Time_Updata1();
    Xinfeng_Init();
    TaskTimerInit();
    RS485_ControlLineInit();
    USART_Initialize(1, BaudRateList[SetData[BaudRate1]]);
    USART_Initialize(2, BaudRateList[SetData[BaudRate2]]);
    USART_Initialize(3, BaudRateList[SetData[BaudRate3]]);
    
    GPIO_ResetBits(GPIOC,  GPIO_Pin_8);//开启空调电源
    //LCD初始化
    LCD_PortInit();
    LCD_Initinal1();
    TransferData(0x01, 0);  
    delay_ms(1);
    ClearGraphic();    
    // 
    
    Err_Init();
    ErrorList[GetTempErr]=GetSHTCData();//读取传感器的温湿度
    CalculateAverageTemp();//计算平均温度
    CalculateAverageHumi();//计算平均湿度
    
    Aircondition_Search(1);//开始搜索空调
    
    PageJumpTo(1);
    
    while(1)
    {
        IWDG_Feed(); //喂狗
        XinfengStateRead();  //读取新风系统的LED状态
        ADUpdataTask();     //用来发送空调命令的状态机
        
        //
        debugtask();    //用串口模拟出来的空调。地址003
        //
        
        //屏幕操作
        if(Semaphores[Key_Set] == 1)
        {
            Semaphores[Key_Set] = 0;
            if(NowStruct.Switch != NULL)
                NowStruct.Switch(Key_Set);  
        }
        if(Semaphores[Key_Return] == 1)
        {
            Semaphores[Key_Return] = 0;            
            if(NowStruct.Switch != NULL)
                NowStruct.Switch(Key_Return);
        }
        if(Semaphores[Key_Up] == 1 || Semaphores[Key_Up_Long] == 1 )
        {
            Semaphores[Key_Up] = 0;
            Semaphores[Key_Up_Long] = 0;
            if(NowStruct.Switch != NULL)
                NowStruct.Switch(Key_Up);            
            
        }
        if(Semaphores[Key_Down] == 1 || Semaphores[Key_Down_Long] == 1 )
        {
            Semaphores[Key_Down] = 0;
            Semaphores[Key_Down_Long] = 0;
            if(NowStruct.Switch != NULL)
                NowStruct.Switch(Key_Down); 
        }
        
        if(_1sflag) //定时一秒的任务 闪错误灯
        {
            _1sflag = 0;
            errtest = 0;
            RunTime += 1;
            for(i=0;i<Total4;i++)
            {
                errtest += ErrorList[i];
            }
            if(errtest)
            {
                GPIO_ToggleBits(LED4);
            }
            else GPIO_SetBits(LED4);
        }
        
        if(_2sflag) //2秒任务  获取温湿度并计算新的控制状态
        {
            _2sflag = 0;
            
            if(Nowpage == 1 || Nowpage == 20)
                PageUpdata();
            I2C_Num = 1;
            ErrorList[GetTempErr]=GetSHTCData();
            CheckTask();
            if(StateList[PowerOff] == 0)
            {
                ControlTask();
            }
        }
        if(tempUpdataCnt >= 10)//10s获取一次空调的温湿度 
        {
            Aircondition_GetAnologDataAll(1);
            tempUpdataCnt = 0;
        }
        if(_5sflag) //每五秒更新控制器的时间计数器 
        {
            _5sflag = 0;
            
            washingtime += 5;
            if(washingtime>=86400) //控制着清洗周期
            {
                SetData[WorkDays]+=1;
                if(SetData[WorkDays] >= SetData[WashingInterval])
                {
                    SetData[WorkDays] = 0;
                    if(StateList[Xinfengqingxi] == 0)
                    {
                        Xinfeng_Qingxi(1);
                    }
                }
                STMFLASH_Write(FLASH_SAVE_ADDR, (u32*)SetData, sizeof(SetData)/4);
                washingtime = 0;
            }
            
            if(StateList[Xinfengqingxi] == 1)
            {
                if(washingtime >= 600)
                {
                    washingtime = 0;
                    Xinfeng_Qingxi(1);
                }
            }
            
            searchtime += 5;
            if(searchtime > 180)
            {
                Aircondition_Search(1);
                searchtime = 0;
            }
        }
    }
}






