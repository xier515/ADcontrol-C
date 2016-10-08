#include "RS485.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "string.h"
#include "delay.h"
#include "dme3000.h"
#include "communication.h"
#include "debug.h"

#define EN1 GPIOD,GPIO_Pin_14   //RE是接收使能端低电平有效 DE是发送使能端高电平有效
#define EN2 GPIOD,GPIO_Pin_13
#define EN3 GPIOD,GPIO_Pin_12

unsigned int BaudRateList[6]={0, 2400, 4800, 9600, 14400, 19200};

void RS485_ControlLineInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14); //开机时必须让芯片处于接收状态。
}
void USART1_Initialize(u32 bound) 
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
														  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //PA9 USART1_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //PA10 USART1_RX

															   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 //USART1 端口初始化

										   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	USART_Cmd(USART1, ENABLE);  //使能串口1 

	USART_ClearFlag(USART1, USART_FLAG_TC);


	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

												  //Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	NVIC_Init(&NVIC_InitStructure);	


}

void USART2_Initialize(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);


	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART1); //PA2 USART2_TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART1); //PA3 USART2_RX


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 //USART2 端口初始化

													 //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2

	USART_Cmd(USART2, ENABLE);  //使能串口2

	USART_ClearFlag(USART2, USART_FLAG_TC);


	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

												  //Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


}

void USART3_Initialize(u32 bound)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART1);  //PB10 USART3_TX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART1);	//PB11 USART3_RX


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);			 //USART2 端口初始化

													 //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口2

	USART_Cmd(USART3, ENABLE);  //使能串口2

	USART_ClearFlag(USART3, USART_FLAG_TC);


	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

												  //Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


}

void USART_Initialize(u8 SerialNumber, u32 bound)
{
	switch (SerialNumber)
	{
        case 1:
        {
            USART_DeInit(USART1);
            USART1_Initialize(bound);
            break;
        }
        case 2:
        {
            USART_DeInit(USART2);
            USART2_Initialize(bound);
            break;
        }
        case 3:
        {
        {
            USART_DeInit(USART3);
            USART3_Initialize(bound);
            break;
        }
        }
        default: break;
	}
}

void USART_SendChars(u8 SerialNumber, char* Chars, u8 length)
{
    u8 i;
    if(SerialNumber == 1)
    {
        GPIO_SetBits(EN1);
        
        for(i=0; i<length; i++)
        {            
            while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
            USART_SendData(USART1, *(Chars+i));
        }
        while(!USART_GetFlagStatus(USART1,USART_FLAG_TC));
        
        GPIO_ResetBits(EN1);
    }
    else if(SerialNumber == 2)
    {
        GPIO_SetBits(EN2);
        for(i=0; i<length; i++)
        {
            while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
            USART_SendData(USART2, *(Chars+i));
        }
        while(!USART_GetFlagStatus(USART2,USART_FLAG_TC));
        GPIO_ResetBits(EN2);
    }
    else if(SerialNumber == 3)
    {
        GPIO_SetBits(EN3);
        for(i=0; i<length; i++)
        {
            while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
            USART_SendData(USART3, *(Chars+i));
        }
        while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));
        GPIO_ResetBits(EN3);
    }
}
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	char Res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART1);//(USART1->DR);	//读取接收到的数据
        AD_ReceiveACK(Res);
        //while(!USART_GetFlagStatus(USART3,USART_FLAG_TC));//转发串口1的数据到串口3
        //USART_SendChars(3, &Res, 1);
    }
 		 
} 

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART2);//(USART1->DR);	//读取接收到的数据
        Receive_Debug(Res);
    }
    Res = Res;
 		 
} 

void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	u8 Res;

	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
	{
		Res = USART_ReceiveData(USART3);//(USART1->DR);	//读取接收到的数据
        UserCMD_Receive(Res);
    }
 		 
    Res = Res;
} 

