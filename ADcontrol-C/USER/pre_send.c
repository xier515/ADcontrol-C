/************************************
pre_send.c实现了发送前的字符串预处理。
十六进制、字符型转换，和校验码计算等功能

************************************/
#include "pre_send.h"
#include "DME3000.H"
#include "rs485.h"


int NowADDR = 0;
int NowCMD = 0;
int CMDState = 0;
int TimeOutCnt = 0;
int CMDBusy = 0;

char AD_CMD[60]="~20014043E00200";

void HextoChar(char *addr,u8 hex)
{
    if(hex>=16) return;
    
    if(hex<10)
        *addr = hex + '0';
    else if(hex>=10)
    {
        hex -=10;
        *addr = hex + 'A';
    }
}
char CharToHex(char Char)
{
    if(Char >= '0' && Char <= '9')
    {
        return (Char - '0');
    }
    else if(Char >= 'A' && Char <= 'F')
    {
        return (Char - 'A' +10);
    }
    else 
        return 0xff;
}
/*****************************************
u16 LENGTHAddLCHKSUM(u16 LENGTH)
void AddCHKSUM(u16 LENGTH)
生成校验和的函数
*****************************************/
void LENGTHAddLCHKSUM(u16 length)
{
    u16 a1,a2,a3,sum;
    a1 = length & 0x000f;
    a2 = (length >>4) & 0x000f;
    a3 = (length >>8) & 0x000f;
    sum = (a1 + a2 + a3) % 16;
    sum = ~sum;
    sum = sum + 1;
    length += (sum << 12);
    HextoChar(AD_CMD+9, ((length>>12) & 0x000f) );
    HextoChar(AD_CMD+10, ((length>>8) & 0x000f) );
    HextoChar(AD_CMD+11, ((length>>4) & 0x000f) );
    HextoChar(AD_CMD+12, ((length) & 0x000f) );
}

void AddCHKSUM(u16 length)
{
    u16 sum=0,i;
    for(i=1; i<=12+length; i++)
    {
        sum += AD_CMD[i];
        
    }
//    sum = 0x02e1;
    sum = ~sum;
    sum = sum + 1; 
    
    HextoChar(AD_CMD+13+length, ((sum>>12) & 0x000f) );
    HextoChar(AD_CMD+14+length, ((sum>>8) & 0x000f) );
    HextoChar(AD_CMD+15+length, ((sum>>4) & 0x000f) );
    HextoChar(AD_CMD+16+length, ((sum) & 0x000f) );
    AD_CMD[17+length] = _EOI;
}


/*****************************************
u16 LENGTHAddLCHKSUM(u16 LENGTH)
void AddCHKSUM(u16 LENGTH)
检验校验和函数，并返回info长度。
*****************************************/
int CheckCHKSUM(char *buf)
{
    u8 i;
    u16 sum,sum1,a1,a2,a3,a4,length;
     
    //检验length段的校验和 否则返回-1   
    sum = CharToHex(buf[9]);
    a1 = CharToHex(buf[10]);
    a2 = CharToHex(buf[11]);
    a3 = CharToHex(buf[12]);

    sum1 = (a1 + a2 + a3) % 16;
    sum1 = ~sum1;
    sum1 = sum1 + 1;
    sum1 = sum & 0x000f;
    if(sum != sum1) return -1;
    
    length = (a1<<8) + (a2<<4) + a3;
    
    //以下检验段尾的校验和 否则返回-2
    a1 = CharToHex(buf[13+length]);
    a2 = CharToHex(buf[14+length]);
    a3 = CharToHex(buf[15+length]);
    a4 = CharToHex(buf[16+length]);

    sum = (a1<<12) + (a2<<8) + (a3<<4) + a4;
    
    sum1 = 0;
    for(i=1; i<=12+length; i++)
    {
        sum1 += buf[i];
    }
    sum1 = (~sum1) + 1;
    
    if(sum != sum1) return -2;
    
    return length;
    
}

void CMD_Send(u8 adr, u8 cid2, char* Info, u8 lenid)
{
    int i;
    while(CMDBusy);
    for(i=0; i<60; i++)
    {
        AD_CMD[i]=0x00;
    }
    AD_CMD[0] = _SOI;
    HextoChar(AD_CMD+1,(_VER>>4)&0x0f);
    HextoChar(AD_CMD+2,_VER&0x0f);
    HextoChar(AD_CMD+3,(adr>>4)&0x0f);
    HextoChar(AD_CMD+4,adr&0x0f);
    HextoChar(AD_CMD+5,(_CID1>>4)&0x0f);
    HextoChar(AD_CMD+6,_CID1&0x0f);
    HextoChar(AD_CMD+7,(cid2>>4)&0x0f);
    HextoChar(AD_CMD+8,cid2&0x0f);
    LENGTHAddLCHKSUM(lenid);
    if(Info != 0)
    {
        for(i=0; i<lenid; i++)
        {
            AD_CMD[13+i] = *(Info+i);
        }
    }
    AddCHKSUM(lenid);
    USART_SendChars(AirCondition, AD_CMD, 13+lenid+5);
    NowCMD = cid2;
    NowADDR = adr;
    CMDBusy = 1;
    TimeOutCnt = 0;
    CMDState = 0;
}

void CMD_Send_Debug(u8 adr, u8 cid2, char* Info, u8 lenid)
{
    int i;
    for(i=0; i<60; i++)
    {
        AD_CMD[i]=0x00;
    }
    AD_CMD[0] = _SOI;
    HextoChar(AD_CMD+1,(_VER>>4)&0x0f);
    HextoChar(AD_CMD+2,_VER&0x0f);
    HextoChar(AD_CMD+3,(adr>>4)&0x0f);
    HextoChar(AD_CMD+4,adr&0x0f);
    HextoChar(AD_CMD+5,(_CID1>>4)&0x0f);
    HextoChar(AD_CMD+6,_CID1&0x0f);
    HextoChar(AD_CMD+7,(cid2>>4)&0x0f);
    HextoChar(AD_CMD+8,cid2&0x0f);
    LENGTHAddLCHKSUM(lenid);
    if(Info != 0)
    {
        for(i=0; i<lenid; i++)
        {
            AD_CMD[13+i] = *(Info+i);
        }
    }
    AddCHKSUM(lenid);
    USART_SendChars(2, AD_CMD, 13+lenid+5);
}

void CMD_Resp_User(u8 adr, u8 cid2, char* Info, u8 lenid)
{
    int i;
    for(i=0; i<60; i++)
    {
        AD_CMD[i]=0x00;
    }
    AD_CMD[0] = _SOI;
    HextoChar(AD_CMD+1,(_VER>>4)&0x0f);
    HextoChar(AD_CMD+2,_VER&0x0f);
    HextoChar(AD_CMD+3,(adr>>4)&0x0f);
    HextoChar(AD_CMD+4,adr&0x0f);
    HextoChar(AD_CMD+5,(_CID1>>4)&0x0f);
    HextoChar(AD_CMD+6,_CID1&0x0f);
    HextoChar(AD_CMD+7,(cid2>>4)&0x0f);
    HextoChar(AD_CMD+8,cid2&0x0f);
    LENGTHAddLCHKSUM(lenid);
    if(Info != 0)
    {
        for(i=0; i<lenid; i++)
        {
            AD_CMD[13+i] = *(Info+i);
        }
    }
    AddCHKSUM(lenid);
    USART_SendChars(3, AD_CMD, 13+lenid+5);
}

void CMD_3to1_Send(char* Info, u8 length)
{
    while(CMDBusy);
    CMDBusy = 1;
    USART_SendChars(AirCondition, Info, length);//转发到串口1
    CMDBusy = 0;
}

void CMD_1to3_Send(char* Info, u8 length)
{
    while(CMDBusy);
    CMDBusy = 1;
    USART_SendChars(User, Info, length);//转发到串口3
    CMDBusy = 0;
}

