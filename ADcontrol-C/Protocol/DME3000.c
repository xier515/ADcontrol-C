#include <stm32f4xx.h>
#include "messagebox.h"
#include "dme3000.h"
#include "pre_send.h"


#define INFOOFFSET 13
u8 SearchFin = 0;

/*****************************************
void Aircondition_GetAnologData()
�ӿյ���ȡ�����¶ȣ�����ʪ�Ⱥ������¶�
*****************************************/
void Aircondition_GetAnologData(u8 Adr)
{
    CMD_Send(Adr, 0x42, 0, 0);

}

void Aircondition_GetAnologData_Receive(u8 Adr, char* buf)
{
    ADList[Adr].tempInside  = (CharToHex(buf[INFOOFFSET+0])<<12) + (CharToHex(buf[INFOOFFSET+1])<<8) + (CharToHex(buf[INFOOFFSET+2])<<4) + CharToHex(buf[INFOOFFSET+3]);
    ADList[Adr].tempInside /= 10;
    ADList[Adr].humi = (CharToHex(buf[INFOOFFSET+4])<<12) + (CharToHex(buf[INFOOFFSET+5])<<8) + (CharToHex(buf[INFOOFFSET+6])<<4) + CharToHex(buf[INFOOFFSET+7]);
    ADList[Adr].humi /= 10;
    ADList[Adr].tempOutside  = (CharToHex(buf[INFOOFFSET+8])<<12) + (CharToHex(buf[INFOOFFSET+9])<<8) + (CharToHex(buf[INFOOFFSET+10])<<4) + CharToHex(buf[INFOOFFSET+11]);
    ADList[Adr].tempOutside /= 10;
}
void Aircondition_GetAnologDataAll(u8 start)
{
    static int i=256;
    if(start)
    {
        i = 1;
    }
    if(i<=254)
    {
        if(ADList[i].EN == 1)
        {
            Aircondition_GetAnologData(i);
        }
        i++;
    }
    
}
/*****************************************
void Aircondition_OnOff()
���ƿյ���Դ�Ŀ���
*****************************************/
void Aircondition_OnOff(u8 Adr, u8 En)
{
    char info[2];
    if(En == 1)//��������
    {
        info[0] = '1';
        info[1] = '0';
        CMD_Send(Adr, 0x45, info, 2);
        ADList[Adr].State = 1;
    }
    else if(En == 0)//�ػ�����
    {
        info[0] = '1';
        info[1] = 'F';
        CMD_Send(Adr, 0x45, info, 2);
        ADList[Adr].State = 0;        
    }
}
u8 OnorOff = 0;
void Aircondition_AllOn(u8 start)
{
    static int i=256;
    if(start)
    {
        i = 1;
    }
    if(i<=255)
    {
        if(ADList[i].EN == 1)
        {
            if(ADList[i].State != 1)
            {
                OnorOff = 1;
                Aircondition_OnOff(i,1);
            }
        }
        i++;
    }
    
}
void Aircondition_AllOff(u8 start)
{
    static int i=256;
    if(start)
    {
        i = 1;
    }
    if(i<=255)
    {
        if(ADList[i].EN == 1)
        {
            if(ADList[i].State != 0)
            {
                OnorOff = 0;
                Aircondition_OnOff(i,0);
            }
        }
        i++;
    }
    
}
void Aircondition_OnOff_Receive(u8 Adr)
{
//     if(OnorOff == 0)
//         ADList[Adr].State = 0;
//     else if(OnorOff == 1)
//         ADList[Adr].State = 1;
}

//��ȡ���ػ�״̬��ͬʱ���Լ��յ��Ƿ���ڡ�
void Aircondition_CMD43(u8 Adr)
{
    CMD_Send(Adr, 0x43, 0, 0);  
}
void Aircondition_CMD43_Receive(u8 Adr, char* buf)
{
    ADList[Adr].EN = 1;
    if((CharToHex(buf[INFOOFFSET+0])<<4) + (CharToHex(buf[INFOOFFSET+1])<<0) == 0x01)
    {
        ADList[Adr].State = 1;
    }
    else
    {
        ADList[Adr].State = 0;
    }
}


//��ȡ����״̬
void Aircondition_GetState(u8 Adr)
{
    CMD_Send(Adr, 0x82, 0, 0);  
}
void Aircondition_GetState_Receive(u8 Adr, char* buf)
{
    
}

/*****************************************
void Aircondition_SetTemp(u8 Adr)
�յ��¶��趨��
void Aircondition_SetHum(u8 Adr)
���ÿյ�ʪ���趨��
*****************************************/
void Aircondition_SetTemp(u8 Adr)
{
    char info[6];
    int temp = SetData[SetTemp] * 10;
    info[0] = '8';
    info[1] = '2';
    HextoChar(info+2, (temp>>12) & 0x0f);
    HextoChar(info+3, (temp>>8) & 0x0f);
    HextoChar(info+4, (temp>>4) & 0x0f);
    HextoChar(info+5,temp & 0x0f); 
    
    CMD_Send(Adr, 0x49, info, 6);  
    ADList[Adr].settingTemp= SetData[SetTemp];    
}
void Aircondition_SetTempAll(u8 start)
{
    static int i=256;
    if(start)
    {
        i = 1;
    }
    if(i<=255)
    {
        if(ADList[i].EN == 1)
        {
            Aircondition_SetTemp(i);
        }
        i++;
    }
}
void Aircondition_SetHum(u8 Adr)
{
    char info[6];
    int humidity = SetData[LimitHumidity] * 10;
    info[0] = '8';
    info[1] = '4';
    HextoChar(info+2, (humidity>>12) & 0x0f);
    HextoChar(info+3, (humidity>>8) & 0x0f);
    HextoChar(info+4, (humidity>>4) & 0x0f);
    HextoChar(info+5,humidity & 0x0f); 
    CMD_Send(Adr, 0x49, info, 6); 
    ADList[Adr].settingHumi= SetData[LimitHumidity];
}
void Aircondition_SetHumAll(u8 start)
{
    static int i=256;
    if(start)
    {
        i = 1;
    }
    if(i<=255)
    {
        if(ADList[i].EN == 1)
        {
            Aircondition_SetHum(i);
        }
        i++;
    }
}
void Aircondition_Set_Receive(u8 Adr)
{
    //���봦��
}


/*****************************************
void Aircondition_GetVersion(u8 Adr)
��ȡ�յ���Э��汾��
��������̽��յ��Ƿ����
�ӵ�ַ1��254������ѯ �л�Ӧ��ע����ADList��
*****************************************/
void Aircondition_GetVersion(u8 Adr)
{
    CMD_Send(Adr, 0x4F, 0, 0);   
}
void Aircondition_GetVersion_Receive(u8 Adr)
{
    ADList[Adr].EN = 1;
}
void Aircondition_Search(u8 start)
{ 
    static int i=256;
    if(start)
    {
        i = 1;
    }
    if(i<=254)
    {
        Aircondition_CMD43(i);
//        Aircondition_GetVersion(i); //����ʹ��CMD43��̽��յ��Ĵ���
        i++;
    }
    if(i==255)
    {
        SearchFin = 1;
        Aircondition_SetTempAll(1);
        Aircondition_SetHumAll(1);
        i++;
    }
}

/*****************************************
void AD_ReceiveACK(char data)
�����жϴ�����
*****************************************/
unsigned char userReqADDR = 0;
void AD_ReceiveACK(char data)
{
    static char Usart1_Buff[50];
    static int cnt=0;
    int i,length,adr;
    TimeOutCnt = 0;
    if(data == _SOI)        //���յ���ʼ�ź�
    {
        for(i=0; i<50; i++)
        {
            Usart1_Buff[i] = 0;    // ��ʼ������
        }
        cnt = 0;            //������ռ�����
        Usart1_Buff[cnt] = data;        
    }
    else if(data == _EOI)       //���ܵ������ź�
    {
        CMDBusy = 0;
        TimeOutCnt = 0;
        Usart1_Buff[cnt] = data;
        length = CheckCHKSUM(Usart1_Buff);      //����У���
        if(length>=0)   //���У�����ȷ
        {
            adr = (CharToHex(Usart1_Buff[3])<<4)+ CharToHex(Usart1_Buff[4]);
            i = (CharToHex(Usart1_Buff[7])<<4)+ CharToHex(Usart1_Buff[8]);  //���Ӧ���Ƿ����쳣
            if(i==0) //���쳣
            {
                ErrorList[ADControlErr] = 0;
                switch(NowCMD)
                {
                    case 0x42: Aircondition_GetAnologData_Receive(adr, Usart1_Buff); break;
                    case 0x43: Aircondition_CMD43_Receive(adr, Usart1_Buff);break;
                    case 0x45: Aircondition_OnOff_Receive(adr);break;
                    case 0x47: break;
                    case 0x49: Aircondition_Set_Receive(adr); break;
                    case 0x4D: break;
                    case 0x4F: Aircondition_GetVersion_Receive(adr); break;
                    case 0x82: break;
                }
                if(userReqADDR == adr)
                {
                    userReqADDR = 0;
                    CMD_1to3_Send(Usart1_Buff, cnt+1);
                }
            }
            else //���쳣,�����쳣����
            {
                ErrorList[ADControlErr] = 1;
                switch(i)
                {
                    case 0x01: break;
                    case 0x02: break;
                    case 0x03: break;
                    case 0x04: break;
                    case 0x05: break;
                    case 0x06: break;
                }
            }
        }
        else // У��ʹ���, �쳣����
        {
            
        }
    }
    else  //�����յ��ǿ�ͷ�ǽ�β�ַ�ʱ
    {
        Usart1_Buff[cnt] = data;
    }
    
    cnt++;
    if(cnt>=49) cnt=0;
}

void ADUpdataTask()
{
    Aircondition_Search(0);
    Aircondition_GetAnologDataAll(0);
    Aircondition_AllOff(0);
    Aircondition_AllOn(0);
    Aircondition_SetTempAll(0);
    Aircondition_SetHumAll(0);
}

