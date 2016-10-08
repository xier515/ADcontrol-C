#include "logic.h"
#include "messagebox.h"
#include "DME3000.h"


//计算温度平均值 返回有效温度值个数
u8 CalculateAverageTemp(void)
{
    int i;
    int sum=0,cnt=0,live=0;
    if(StateList[RoomTemp]<50 && StateList[RoomTemp]>-50)
    {
        sum += StateList[RoomTemp];
        cnt++;
    }
    for(i=0; i<256; i++)
    {
        if((ADList+i)->EN == 1)
        {
            live++;
            if((ADList+i)->tempInside<50 && (ADList+i)->tempInside>-50)
            {
                sum += (ADList+i)->tempInside;
                cnt++;
            }
        }
    }
    StateList[AverageTemp] = sum/cnt;
    if(live == 0) ErrorList[NoAD] = 1;
    else    ErrorList[NoAD] = 0;
    return cnt;
}
//计算湿度平均值 返回有效湿度值个数
u8 CalculateAverageHumi(void)
{
    int i;
    int sum=0,cnt=0;
    if(StateList[RoomHumi]<=100 && StateList[RoomHumi]>0)
    {
        sum += StateList[RoomHumi];
        cnt++;
    }
    for(i=0; i<256; i++)
    {
        if((ADList+i)->EN == 1)
        {
            if((ADList+i)->humi<=100 && (ADList+i)->humi>0)
            {
                sum += (ADList+i)->humi;
                cnt++;
            }
        }
    }
    StateList[AverageHumi] = sum/cnt;
    return cnt;
}

void HumidityCheck()
{
    int i=0;
    static int lasthumi=0,lasttime=0 , _12htime=0;
    if(StateList[NeedDehumidity] == 1)
    {
        if(RunTime - lasttime >= 1800)
        {
            if(StateList[AverageHumi] >= lasthumi)
            {
                ErrorList[DeHumiErr] = 1;
                _12htime = RunTime + 43200;
            }
        }
    }
    if(ErrorList[DeHumiErr] == 1)
    {
        if(RunTime >= _12htime )
            ErrorList[DeHumiErr] =0;
    }
    if(StateList[AverageHumi] > SetData[LimitHumidity])
    {

        if(StateList[NeedDehumidity] == 0)
        {
            lasthumi = StateList[AverageHumi];
            lasttime = RunTime;
        }
        if(ErrorList[DeHumiErr] == 0)
        {
            StateList[NeedDehumidity] = 1;
        }
        else
        {
            StateList[NeedDehumidity] = 0;
        }
    }
    else if(StateList[AverageHumi] <= SetData[LimitHumidity] - 5)
    {
        ErrorList[DeHumiErr] = 0;
        StateList[NeedDehumidity] = 0;
    }
}

void TempCheck()
{
    static int qiangzhi = 0,Lastqiangzhi=0;
    
    if(RunTime-Lastqiangzhi >= 18000) ErrorList[LowEfficiency] = 0;  
    if(SetData[SetTemp] - StateList[OutsideTemp] >= 1) //室外温度较低时
    {
        if(StateList[AverageTemp] >= (SetData[SetTemp] + SetData[LimitTemp])) //达到强制启动温度
        {
            qiangzhi = 1;
            if(RunTime >= 7200)
            {
                ErrorList[LowEfficiency] = 1;
                Lastqiangzhi = RunTime;
            }
            StateList[NeedADCool] = 1;
            if(StateList[OutsideTemp]>15)
            {
                StateList[NeedXinfengCool] = 2; //需要新风系统水冷
            }
            else
            {
                StateList[NeedXinfengCool] = 1; //需要新风系统通风
            }
        }
        else if(StateList[AverageTemp] >= SetData[SetTemp]+1) //当
        {

            if(StateList[OutsideTemp]>15)
            {
                StateList[NeedXinfengCool] = 2; //需要新风系统水冷
            }
            else
            {
                StateList[NeedXinfengCool] = 1; //需要新风系统通风
            }
        }
        else if(StateList[AverageTemp] <= SetData[SetTemp]-2)
        {
            StateList[NeedXinfengCool] = 0;
            StateList[NeedADCool] = 0;
        }
        if(qiangzhi == 1)
        {
            if(StateList[AverageTemp] <= SetData[SetTemp]+1)
            {
                StateList[NeedADCool] = 0;
                qiangzhi = 0;
            }
        }
    }
    else//室外温度较高时
    {
        StateList[NeedADCool] = 1;
        StateList[NeedXinfengCool] = 0;
    }

}

void CheckTask(void)
{
    CalculateAverageTemp();
    CalculateAverageHumi();
    HumidityCheck();
    TempCheck();
}
