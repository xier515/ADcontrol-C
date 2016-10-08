#ifndef _PRE_SEND_H
#define _PRE_SEND_H

#include <stm32f4xx.h>

#define NO_ERR 0
#define TIMEOUT 1
#define CHKSUMERR 2

extern char AD_CMD[60];
extern int NowADDR;
extern int NowCMD;
extern int CMDBusy;
extern int TimeOutCnt;
extern int CMDState;

void HextoChar(char *addr,u8 hex);
char CharToHex(char Char);
void LENGTHAddLCHKSUM(u16 length);
void AddCHKSUM(u16 length);
int CheckCHKSUM(char *buf);
void CMD_Send(u8 adr, u8 cid2, char* Info, u8 lenid);
void CMD_Send_Debug(u8 adr, u8 cid2, char* Info, u8 lenid);
void CMD_Resp_User(u8 adr, u8 cid2, char* Info, u8 lenid);
void CMD_3to1_Send(char* Info, u8 length);
void CMD_1to3_Send(char* Info, u8 length);
#endif

