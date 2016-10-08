#ifndef __LED_H
#define __LED_H

#include "stm32f4xx.h"

#define KEY_SET GPIOA,GPIO_Pin_0
#define KEY_UP GPIOE,GPIO_Pin_6
#define KEY_DOWN GPIOA,GPIO_Pin_1
#define KEY_RETURN GPIOE,GPIO_Pin_5

#define KEY_QUESHUI	GPIOB,GPIO_Pin_0
#define KEY_TONGFENG	GPIOB,GPIO_Pin_1
#define KEY_SHUILENG	GPIOE,GPIO_Pin_7
#define KEY_QINGXI	GPIOE,GPIO_Pin_8


#define LED1	GPIOB,GPIO_Pin_12
#define LED2	GPIOB,GPIO_Pin_13
#define LED3	GPIOB,GPIO_Pin_14
#define LED4	GPIOB,GPIO_Pin_15

#define LED5	GPIOD,GPIO_Pin_8
#define LED6	GPIOD,GPIO_Pin_9
#define LED7	GPIOD,GPIO_Pin_10
#define LED8	GPIOD,GPIO_Pin_11

void MyLED_Init(void); 
void MyKey_Init(void);
u8 Key_Read(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
