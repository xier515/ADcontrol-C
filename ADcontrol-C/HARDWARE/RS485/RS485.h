#ifndef _RS485_h
#define _RS485_h

#include "stm32f4xx.h"

#define AirCondition 1
#define Xinfeng     2
#define User    3



extern unsigned int BaudRateList[6];
extern char BufOfUsart1[17];

void RS485_ControlLineInit(void);
void USART_Initialize(u8 SerialNumber, u32 bound);
void USART_SendChars(u8 SerialNumber, char* Chars, u8 length);

#endif // !_RS485_h
