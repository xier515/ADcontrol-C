#include <stm32f4xx.h>
#include "stm32f4xx_gpio.h"

#define LED1 GPIOB,GPIO_Pin_0
#define LED2 GPIOB,GPIO_Pin_1
#define LED3 GPIOE,GPIO_Pin_7
#define LED4 GPIOE,GPIO_Pin_8

#define RELAY1 GPIOE,GPIO_Pin_10
#define RELAY2 GPIOE,GPIO_Pin_11
#define RELAY3 GPIOE,GPIO_Pin_12
#define RELAY4 GPIOE,GPIO_Pin_13


//旧款新风系统的输入输出端口初始化
void Xinfeng_IOInit()
{
    GPIO_InitTypeDef Xinfeng_IOInitStructure
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOB, ENABLE);
    
    Xinfeng_IOInitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
    Xinfeng_IOInitStructure.GPIO_Mode = GPIO_Mode_OUT;
    Xinfeng_IOInitStructure.GPIO_OType = GPIO_OType_PP;
    Xinfeng_IOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    Xinfeng_IOInitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOE, &Xinfeng_IOInitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13);
    
    Xinfeng_IOInitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 ;
    Xinfeng_IOInitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_Init(GPIOE, &Xinfeng_IOInitStructure);
    
    Xinfeng_IOInitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 ;
    GPIO_Init(GPIOB, &Xinfeng_IOInitStructure);
}

