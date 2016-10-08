#include "communication.h"
#include "pre_send.h"
#include "rs485.h"
#include "string.h"
#include "show.h"
#include "messagebox.h"
#include "Clock.h"
#include "pre_send.h"
#include "dme3000.h"
#include "action.h"

int UserDataLength = 0;
char UserData1[200];
const char Stringno1[] = "控制器状态\r\n";
void UserDataBuilder()
{
    UserDataLength=0;
    memcpy(UserData1+UserDataLength, Stringno1, sizeof(Stringno1));
    UserDataLength+=sizeof(Stringno1);
    
}
/*
void UserCMD_Receive(char data)
{
    int i;
    static char UserCMD[40];
    static int cnt = 0;
    if(data == '~')
    {
        cnt = 0;
        return;
    }
    UserCMD[cnt] = data;
    cnt++;
    if(UserCMD[0] == 's' && UserCMD[1] == 't' && UserCMD[2] == 'a' && UserCMD[3] == 't' && UserCMD[4] == 'e')
    {
        UserDataBuilder();
        USART_SendChars(3, "控制器状态\r\n", 12);
        USART_SendChars(3, (char*)Time_String_YMD,12);
        USART_SendChars(3, "\r\n", 2);
        USART_SendChars(3, (char*)Time_String_HMS,9);
        USART_SendChars(3, "\r\n", 2);
        USART_SendChars(3, "平均室温:", 9);
        USART_SendChars(3, shiwen1_s, 2);
        USART_SendChars(3, "℃\t平均湿度:", 12);
        USART_SendChars(3, shidu1_s, 2);
        USART_SendChars(3, "%rh\t室外温度:", 13);
        USART_SendChars(3, shiwaiwendu, 2);        
        USART_SendChars(3, "℃\r\n设定温度:", 13);
        USART_SendChars(3, shiwen2_s, 2);
        USART_SendChars(3, "℃\t设定湿度:", 12);
        USART_SendChars(3, shidu2_s+1, 3 );
        USART_SendChars(3, "%rh\r\n", 5);
        USART_SendChars(3, "空调强制启动温度: +", 19);
        USART_SendChars(3, qiangzhiqidong, 2);
        USART_SendChars(3, "℃\r\n", 4);
        USART_SendChars(3, "距离上次清洗:", 13);
        USART_SendChars(3, gongzuotianshu_s, 2);
        USART_SendChars(3, "天\r\n", 4);
        USART_SendChars(3, "\r\n", 2);
        for(cnt=0; cnt<40; cnt++)
            UserCMD[cnt] = 0;
        cnt = 0;
    }
    else if(UserCMD[0] == 'l' && UserCMD[1] == 'i' && UserCMD[2] == 's' && UserCMD[3] == 't' )
    {
        USART_SendChars(3, "空调列表\r\n", 10);
        for(i=1; i<=254; i++)
        {
            if(ADList[i].EN == 1)
            {
                USART_SendChars(3, "空调地址:", 9);
                NumToString(UserCMD, i, 3);
                USART_SendChars(3, UserCMD, 3);
                USART_SendChars(3, "\t工作状态:", 10);
                if(ADList[i].State == 1)
                    USART_SendChars(3, "开机", 4);
                else if(ADList[i].State == 0)
                    USART_SendChars(3, "关机", 4);
                USART_SendChars(3, "\r\n传感器温度:", 13);
                NumToString(UserCMD, ADList[i].temp, 2);
                USART_SendChars(3, UserCMD, 2);
                USART_SendChars(3, "℃\t传感器湿度:", 14);
                NumToString(UserCMD, ADList[i].humi, 2);
                USART_SendChars(3, UserCMD, 2);
                USART_SendChars(3, "\r\n", 2);
            }
        }
        for(cnt=0; cnt<40; cnt++)
            UserCMD[cnt] = 0;
        cnt = 0;
    }
    
    
}

*/
void U8to2Char(char *buf, u8 byte)
{
    HextoChar(buf+1, byte & 0x000f);
    HextoChar(buf+0, (byte>>4) & 0x000f);
}

void U16to4Char(char *buf, u16 word)
{
    HextoChar(buf+3, word & 0x000f);
    HextoChar(buf+2, (word>>4) & 0x000f);
    HextoChar(buf+1, (word>>8) & 0x000f);
    HextoChar(buf, (word>>12) & 0x000f);
}
u16 _4ChartoU16(char *buf)
{
    return (CharToHex(buf[0])<<12) + (CharToHex(buf[1])<<8) + (CharToHex(buf[2])<<4) + CharToHex(buf[3]);
}
void User_Report_Temp() //返回控制器的温度、湿度数据
{
    static char buf[13];
    U16to4Char(buf,StateList[AverageTemp]*10);
    U16to4Char(buf+4,StateList[AverageHumi]*10);
    U16to4Char(buf+8,StateList[OutsideTemp]*10);
    buf[12] = '0';
    CMD_Resp_User(SetData[MyAddress],0x00, buf, 13);
    
}

void User_Report_Runstate() //获取控制器开关机状态
{
    static char buf[5];
    if(StateList[PowerOff])
    {
        U16to4Char(buf, 0x0000);
    }
    else
    {
        U16to4Char(buf, 0x0100);
    }
    CMD_Resp_User(SetData[MyAddress],0x00, buf, 4);
}

void User_Report_OK() //回复OK
{

    CMD_Resp_User(SetData[MyAddress],0x00, 0, 0);
}

void User_Report_CMD47()
{
    static char buf[26];
    U16to4Char(buf,(SetData[SetTemp]+SetData[LimitTemp])*10);
    U16to4Char(buf+4,SetData[LimitHumidity]*10);
    U16to4Char(buf+8,SetData[SetTemp]*10);
    U16to4Char(buf+12,SetData[LimitTemp]*10);
    U16to4Char(buf+16,SetData[LimitHumidity]*10);
    U16to4Char(buf+20,0);
    buf[24] = '0';
    buf[25] = '0';
    CMD_Resp_User(SetData[MyAddress],0x00, buf, 26);
}
void User_Report_CMD49(char *info)//设定系统参数 
{
    switch(info[1])
    {
        case '2': SetData[SetTemp] = _4ChartoU16(info+2)/10; break;
        case '3': SetData[LimitTemp] = _4ChartoU16(info+2)/10; break;
        case '4': SetData[LimitHumidity] = _4ChartoU16(info+2)/10; break;
    }
    User_Report_OK();
}
void User_Report_CMD4D()
{
    static char buf[14];
    U16to4Char(buf, NowTime[0]+0x7d0);
    U8to2Char(buf+4, NowTime[1]);
    U8to2Char(buf+6, NowTime[2]);
    U8to2Char(buf+8, NowTime[3]);
    U8to2Char(buf+10, NowTime[4]);
    U8to2Char(buf+12, NowTime[5]);
    CMD_Resp_User(SetData[MyAddress], 0x00, buf, 14);
}

void User_Report_CMD4E(char *info)
{
    SetTime[0] = _4ChartoU16(info) %100;
    SetTime[1] = (CharToHex(info[4])<<4) + CharToHex(info[5]);
    SetTime[2] = (CharToHex(info[6])<<4) + CharToHex(info[7]);
    SetTime[3] = (CharToHex(info[8])<<4) + CharToHex(info[9]);
    SetTime[4] = (CharToHex(info[10])<<4) + CharToHex(info[11]);
    SetTime[5] = (CharToHex(info[12])<<4) + CharToHex(info[13]);
    SaveSetTime();
    User_Report_OK();
}

void User_Report_CMD82()
{
    static char buf[16];
    unsigned char workstate = 0,errstate = 0;
    if(StateList[PowerOff]==0)       workstate += 1<<0;
    if(StateList[NeedXinfengCool])   workstate += 1<<1;
    if(StateList[NeedADCool])        workstate += 1<<2;
    
    if(ErrorList[GetTempErr])   errstate += 1;
    
    U8to2Char(buf,01);
    U8to2Char(buf+2,workstate);
    U8to2Char(buf+4,3);
    U8to2Char(buf+6,0);
    U8to2Char(buf+8,0);
    U8to2Char(buf+10,errstate);
    CMD_Resp_User(SetData[MyAddress],0x00, buf, 12);
}
extern unsigned char userReqADDR;
void UserCMD_Receive(char data)
{
    static char Usart3_Buff[50];
    static int cnt=0;
    int i,length,adr;
    
    cnt++;
    if(cnt>=49) cnt=0;
    
    if(data == _SOI)        //接收到起始信号
    {
        for(i=0; i<50; i++)
        {
            Usart3_Buff[i] = 0;    // 初始化数组
        }
        cnt = 0;            //归零接收计数器
        Usart3_Buff[cnt] = data;        
    }
    else if(data == _EOI)       //接受到结束信号
    {
        Usart3_Buff[cnt] = data; 
        length = CheckCHKSUM(Usart3_Buff);      //计算校验和
        if(length>=0)   //如果校验和正确
        {
            adr = (CharToHex(Usart3_Buff[3])<<4)+ CharToHex(Usart3_Buff[4]);//取得地址码

            i = (CharToHex(Usart3_Buff[7])<<4)+ CharToHex(Usart3_Buff[8]);  //取得命令号
            if(adr == SetData[MyAddress])
            {
                switch(i)
                {
                        case 0x42: User_Report_Temp(); break;
                        case 0x43: User_Report_Runstate(); break;
                        case 0x45: 
                            if(Usart3_Buff[13] == '1' && Usart3_Buff[14] == '0')    StateList[PowerOff] = 0;
                            else if(Usart3_Buff[13] == '1' && Usart3_Buff[14] == 'F')    StateList[PowerOff] = 1;
                            User_Report_OK();break;
                        case 0x47: User_Report_CMD47(); break;
                        case 0x49: User_Report_CMD49(Usart3_Buff+13); break;
                        case 0x4D: User_Report_CMD4D(); break;
                        case 0x4E: User_Report_CMD4E(Usart3_Buff+13); break;
                        case 0x4F: User_Report_OK(); break;
                        case 0x50: User_Report_OK(); break;
                        case 0x82: User_Report_CMD82(); break;
                }
            }
            else
            {
                userReqADDR = adr;
                CMD_3to1_Send(Usart3_Buff, cnt+1);
            }

        }
        else // 校验和错误, 异常处理
        {
            CMD_Resp_User(SetData[MyAddress],0x02,0,0);
        }
    }
    else  //当接收到非开头非结尾字符时
    {
        Usart3_Buff[cnt] = data;
    }
    

}

