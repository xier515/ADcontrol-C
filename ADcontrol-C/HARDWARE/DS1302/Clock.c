#include "messagebox.h"

#include "Clock.h"
#include "DS1302.h"

const signed char Mom[13] = {1,31,28,31,30,31,30,31,31,30,31,30,31};
s8 NowTime[6]={16,2,3,4,57,24};              //存放时间的数组
s8 Time_String_YMD[] = "00年00月00日";    //存放年月日的字符串
s8 Time_String_HMS[] = "00:00:00";        //存放时分秒的字符串
s8 SetTime[6]={16,1,1,0,0,0};

void Clock_Updata(void)
{
    NowTime[ss]++;
    if(NowTime[ss] >= 60)
    {
        NowTime[ss] = 0;
        NowTime[mm]++;
        if(NowTime[mm] >= 60)
        {
            NowTime[mm] = 0;
            NowTime[hh]++;
            if(NowTime[hh] >= 24)
            {
                ReadTimeFromChip();
            }
        }
        shijianyuejie1(NowTime);
    }
    ClockToString();
}

void ClockToString(void)
{
    Time_String_HMS[0] = NowTime[hh]/10 +'0';
    Time_String_HMS[1] = NowTime[hh]%10 +'0';
    Time_String_HMS[3] = NowTime[mm]/10 +'0';
    Time_String_HMS[4] = NowTime[mm]%10 +'0';
    Time_String_HMS[6] = NowTime[ss]/10 +'0';
    Time_String_HMS[7] = NowTime[ss]%10 +'0';
    
    Time_String_YMD[0] = NowTime[YY]/10 +'0';
    Time_String_YMD[1] = NowTime[YY]%10 +'0';
    Time_String_YMD[4] = NowTime[MM]/10 +'0';
    Time_String_YMD[5] = NowTime[MM]%10 +'0';
    Time_String_YMD[8] = NowTime[DD]/10 +'0';
    Time_String_YMD[9] = NowTime[DD]%10 +'0';
}

void ReadTimeFromChip(void)
{
    Time_Updata1();
    Time_Updata0();  
}

void CopyNowToSet(void)
{
    char i;
    for(i=0;i<6;i++)
    {
        SetTime[i] = NowTime[i];
    }
}

void SaveSetTime(void)
{
    char i;
    for(i=0;i<6;i++)
    {
        NowTime[i] = SetTime[i];
    }
    Time_Set(NowTime);
}

void shijianyuejie1(s8* time)
{
//    if(time[ss]>=60)    time[ss] = 0;
//    else if(time[ss]<0)    time[ss] = 59;
    
//    if(time[mm]>=60)    time[mm] = 0;
//    else if(time[mm]<0)    time[mm] = 59;
    
//    if(time[hh]>=24)    time[hh] = 0;
//    else if(time[hh]<0)    time[hh] = 23;   
    
    shijianyuejie2(time+3);
    if(!time[YY]%4)
    {
        if(time[MM] == 2 && time[DD]>29)   time[DD] = 1;
        else if(time[MM] == 2 && time[DD]<1) time[DD] = 29;
        else if((time[MM] != 2) && (time[DD]>=Mom[time[MM]]+1))    time[DD] = 1;
        else if(time[MM] != 2 && time[DD]<1)    time[DD] = Mom[time[MM]];       
    }
    else
    {
        if(time[DD]>=Mom[time[MM]])    time[DD] = 1;
        else if(time[DD]<1)    time[DD] = Mom[time[MM]];           
    }
    if(time[MM]>12)    time[MM] = 1;
    else if(time[MM]<1)    time[MM] = 12; 
    
    if(time[YY]>99)    time[YY] = 0;
    else if(time[YY]<0)    time[YY] = 99; 
}

void shijianyuejie2(s8* time)
{
    if(time[2]>=60)    time[2] = 0;
    else if(time[2]<0)    time[2] = 59;
    
    if(time[1]>=60)    time[1] = 0;
    else if(time[1]<0)    time[1] = 59;
    
    if(time[0]>=24)    time[0] = 0;
    else if(time[0]<0)    time[0] = 23;   
    

}
