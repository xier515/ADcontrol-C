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
    
    GPIO_ResetBits(GPIOC,  GPIO_Pin_8);//�����յ���Դ
    //LCD��ʼ��
    LCD_PortInit();
    LCD_Initinal1();
    TransferData(0x01, 0);  
    delay_ms(1);
    ClearGraphic();    
    // 
    
    Err_Init();
    ErrorList[GetTempErr]=GetSHTCData();//��ȡ����������ʪ��
    CalculateAverageTemp();//����ƽ���¶�
    CalculateAverageHumi();//����ƽ��ʪ��
    
    Aircondition_Search(1);//��ʼ�����յ�
    
    PageJumpTo(1);
    
    while(1)
    {
        IWDG_Feed(); //ι��
        XinfengStateRead();  //��ȡ�·�ϵͳ��LED״̬
        ADUpdataTask();     //�������Ϳյ������״̬��
        
        //
        debugtask();    //�ô���ģ������Ŀյ�����ַ003
        //
        
        //��Ļ����
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
        
        if(_1sflag) //��ʱһ������� �������
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
        
        if(_2sflag) //2������  ��ȡ��ʪ�Ȳ������µĿ���״̬
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
        if(tempUpdataCnt >= 10)//10s��ȡһ�οյ�����ʪ�� 
        {
            Aircondition_GetAnologDataAll(1);
            tempUpdataCnt = 0;
        }
        if(_5sflag) //ÿ������¿�������ʱ������� 
        {
            _5sflag = 0;
            
            washingtime += 5;
            if(washingtime>=86400) //��������ϴ����
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






