#ifndef _DME3000_H
#define _DME3000_H
#include <stm32f4xx.h>

//ͨ���ֶ�����   (���޸ģ�

#define _SOI    0x7E
//��ʼ��־λ

#define _VER    0x21
//ͨ��Э��汾��

#define _CID1   0x60
//�豸����������0x60Ϊ��ɢ�յ���

#define _EOI    0x0D
//����λ

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

