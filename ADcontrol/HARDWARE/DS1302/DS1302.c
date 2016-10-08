#include "stm32f4xx.h"
#include "ds1302.h"
#include "stm32f4xx_gpio.h"
#include "messagebox.h"
#include "delay.h"

extern u8 NowTime[6];

GPIO_InitTypeDef DS1302IOInit;

void DS1302_Pin_Init(void)
{
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
        
    	DS1302IOInit.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
        DS1302IOInit.GPIO_Mode = GPIO_Mode_OUT;
        DS1302IOInit.GPIO_OType = GPIO_OType_PP;
        DS1302IOInit.GPIO_Speed = GPIO_Speed_50MHz;
        DS1302IOInit.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOA, &DS1302IOInit);
    

}

void DS1302_posedge(void)
{

		GPIO_ResetBits(DS1302_SCLK);
		delay_us(1);
		GPIO_SetBits(DS1302_SCLK);
        delay_us(1);
}
void DS1302_negedge(void)
{

		GPIO_SetBits(DS1302_SCLK);
		delay_us(1);
		GPIO_ResetBits(DS1302_SCLK);
        delay_us(1);
}

void DS1302_Write(u8 data)
{
		u8 i;
        DS1302IOInit.GPIO_Mode =  GPIO_Mode_OUT;
        DS1302IOInit.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOA, &DS1302IOInit);
		GPIO_ResetBits(DS1302_SCLK);
        delay_us(1);
		GPIO_SetBits(DS1302_RST);
		delay_us(4);
		for(i=0; i<8; i++)
		{
				if(data&0x01)	GPIO_SetBits(DS1302_IO);
				else GPIO_ResetBits(DS1302_IO);
				
				DS1302_posedge();
				data >>= 1;
		}

}

void DS1302_Write_WithoutStart(u8 data)
{
		u8 i;
        DS1302IOInit.GPIO_Mode =  GPIO_Mode_OUT;
        DS1302IOInit.GPIO_Pin = GPIO_Pin_5;
		GPIO_Init(GPIOA, &DS1302IOInit);
		GPIO_ResetBits(DS1302_SCLK);
        delay_us(1);
		GPIO_SetBits(DS1302_RST);
		delay_us(4);
		for(i=0; i<8; i++)
		{
				if(data&0x01)	GPIO_SetBits(DS1302_IO);
				else GPIO_ResetBits(DS1302_IO);
				
				DS1302_posedge();
				data >>= 1;
		}

}

u8 DS1302_Read(void)
{
		u8 i,data;
    
        DS1302IOInit.GPIO_Pin = GPIO_Pin_5;
        DS1302IOInit.GPIO_Mode =  GPIO_Mode_IN;
		GPIO_Init(GPIOA, &DS1302IOInit);

		delay_us(4);
		for(i=0; i<8; i++)
		{
				data >>= 1;
				DS1302_negedge();
                delay_us(1);
				if(GPIO_ReadInputDataBit(DS1302_IO))	data |= 0x80;
                
				
								
		}
		return data;
}

void DS1302_TransfersEnd(void)
{

		GPIO_ResetBits(DS1302_SCLK);
        delay_us(1);
		GPIO_ResetBits(DS1302_RST);
		delay_us(4);
}




static void DS1302_Wirte_Byte(unsigned char addr, unsigned char data)
{
		DS1302_Write(addr);
		DS1302_Write_WithoutStart( data);
		DS1302_TransfersEnd(); 
		
}

/*********************************
��������Change
������unsigned char i
����ֵ��unsigned char j
���ܣ��ѳ�������ֲַ��DS1302����ĸ�ʽ
		
*********************************/
static unsigned char Change(unsigned char i)
{
    unsigned char j;
    j = i / 10;
    j = j << 4;
    j = j + i % 10;
    return j;
}
/*********************************
��������Time_Set
��������
����ֵ����
���ܣ�д��ʱ��
		
*********************************/

void Time_Set(s8* time)
{
		DS1302_Wirte_Byte(0x8e, 0x00 );
		DS1302_Wirte_Byte(0x8C, Change(time[0]) );
		DS1302_Wirte_Byte(0x88, Change(time[1]) );
		DS1302_Wirte_Byte(0x86, Change(time[2]) );
		DS1302_Wirte_Byte(0x84, Change(time[3]) );
		DS1302_Wirte_Byte(0x82, Change(time[4]) );
		DS1302_Wirte_Byte(0x80, Change(time[5]) );
		DS1302_Wirte_Byte(0x8e, 0x80 );
		
}
/*********************************
��������Time_Updata0
��������
����ֵ����
���ܣ�����ʱ���ʱ �� ��
		
*********************************/

void Time_Updata0(void)
{
		u8 i,n,time;
		for(i=0,n=0x81 ; i<3 ; i+=1,n+=2) //������ȡʱ,��,��
		{
				DS1302_Write(n);   
				time=DS1302_Read();  
				DS1302_TransfersEnd();  
//				NowTime[17-i]=(time>>4)+0x30;  
//				NowTime[18-i]=(time&0x0f)+0x30;
                NowTime[5-i] = (time>>4)*10 + (time&0x0f);
		}
}

/*********************************
��������Time_Updata1
��������
����ֵ����
���ܣ�����ʱ����� �� ��
		
*********************************/
void Time_Updata1(void)
{
		signed char time;
		
		DS1302_Write(0x87);   
		time=DS1302_Read();

		DS1302_TransfersEnd();   
//		NowTime[8]=(time>>4)+0x30;  
//		NowTime[9]=(time&0x0f)+0x30;
		NowTime[2] = (time>>4)*10 + (time&0x0f);
        
		DS1302_Write(0x89);   
		time=DS1302_Read();
		DS1302_TransfersEnd();  
//		NowTime[5]=(time>>4)+0x30;  
//		NowTime[6]=(time&0x0f)+0x30;
		NowTime[1] = (time>>4)*10 + (time&0x0f);
        
		DS1302_Write(0x8D);   
		time=DS1302_Read();	
		DS1302_TransfersEnd();  
//		NowTime[2]=(time>>4)+0x30;  
//		NowTime[3]=(time&0x0f)+0x30;
        NowTime[0] = (time>>4)*10 + (time&0x0f);
}



