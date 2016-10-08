#include "debug.h"

char goback=0;

void debugtask()
{
    if(goback == 1)
    {
        CMD_Send_Debug(0x03,0,0,0);
        goback = 0;
    }
    else if(goback == 2)
    {
        CMD_Send_Debug(0x03,0,"00FA02580258",12);
        goback = 0;
    }
    else if(goback == 3)
    {
        CMD_Send_Debug(0x03,0,"0100",4);
        goback = 0;
    }
}
void Receive_Debug(char data)
{
    static char Usart1_Buff[50];
    static int cnt=0;
    int i,length,adr;
    if(data == _SOI)        //接收到起始信号
    {
        for(i=0; i<50; i++)
        {
            Usart1_Buff[i] = 0;    // 初始化数组
        }
        cnt = 0;            //归零接收计数器
        Usart1_Buff[cnt] = data;        
    }
    else if(data == _EOI)       //接受到结束信号
    {
        
        length = CheckCHKSUM(Usart1_Buff);      //计算校验和
        if(length>=0)   //如果校验和正确
        {
            adr = (CharToHex(Usart1_Buff[3])<<4)+ CharToHex(Usart1_Buff[4]);
            if(adr != 0x03) return; //地址不匹配直接抛弃
            i = (CharToHex(Usart1_Buff[7])<<4)+ CharToHex(Usart1_Buff[8]);  //取得命令号

            switch(i)
            {
                case 0x42: goback=2; break;
                case 0x43: goback=3; break;
                case 0x45: goback=1;break;
                case 0x47:  break;
                case 0x49: goback=1; break;
                case 0x4D: break;
                case 0x4F: goback=1; break;
                case 0x82: break;
            }
            

        }
        else // 校验和错误, 异常处理
        {
            
        }
    }
    else  //当接收到非开头非结尾字符时
    {
        Usart1_Buff[cnt] = data;
    }
    
    cnt++;
    if(cnt>=49) cnt=0;
}

