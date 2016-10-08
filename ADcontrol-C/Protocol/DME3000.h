#ifndef _DME3000_H
#define _DME3000_H
#include <stm32f4xx.h>

//通用字段设置   (可修改）

#define _SOI    0x7E
//起始标志位

#define _VER    0x21
//通信协议版本号

#define _CID1   0x60
//设备类型描述（0x60为分散空调）

#define _EOI    0x0D
//结束位

extern char AD_CMD[60];


void AD_ReceiveACK(char data);

void Aircondition_AllOn(u8 start);
void Aircondition_AllOff(u8 start);
void Aircondition_Search(u8 start);
void Aircondition_SetTempAll(u8 start);
void Aircondition_SetHumAll(u8 start);
void Aircondition_GetAnologDataAll(u8 start);
void ADUpdataTask(void);
#endif

