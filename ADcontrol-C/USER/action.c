#include "action.h"
#include "messagebox.h"
#include "DME3000.H"
#include "OldXinfeng.h"
#include "led.h"

extern unsigned int RunTime;
extern unsigned int xinfeng15s;
void ControlAction_Xinfeng()
{
    static u32 retrycnt=0;
    if(StateList[NeedXinfengCool] != 0)
    {
        
            if(StateList[NeedXinfengCool] == 1)
            {
                if(StateList[Xinfengtongfeng] == 0)
                {
                    Xinfeng_Tongfeng(1);
                    retrycnt++;
                }
                else 
                    retrycnt = 0;
            }
            else if(StateList[NeedXinfengCool] == 2)
            {
                if(StateList[Xinfengshuileng] == 0 && StateList[Xinfengqueshui] == 0)
                {
                    Xinfeng_Shuileng(1);
                    retrycnt++;	
                }
                else if(StateList[Xinfengshuileng] == 0 && StateList[Xinfengqueshui] == 1)
                {
                    Xinfeng_Tongfeng(1);
                    retrycnt++;	
                }
                else
                    retrycnt = 0;
								
            } 
    }
    else
    {
        if(StateList[Xinfengshuileng] || StateList[Xinfengtongfeng])
        {
            Xinfeng_Kaiguan(1);
            retrycnt++;
        }
        else
            retrycnt = 0;
    }
    if(retrycnt >=5)
    {
        ErrorList[XFSLControlErr] = 1;
    }
    else
    {
        ErrorList[XFSLControlErr] = 0;
    }
    
    if(StateList[NeedWash] && (!StateList[Xinfengqingxi]))
    {
        Xinfeng_Qingxi(1);
    }
    else if((!StateList[NeedWash]) && StateList[Xinfengqingxi])
    {
        Xinfeng_Qingxi(1);
    }
/*
//         if(step == 0)
//         {
//             if(!(StateList[Xinfengqueshui] || StateList[Xinfengtongfeng] || StateList[Xinfengshuileng] || StateList[Xinfengqingxi]))
//                 Xinfeng_Kaiguan(1); //如果关机就开机
//             step = 1;
//         }
//         else if(step == 1)
//         {
//             if((StateList[Xinfengqueshui] || StateList[Xinfengtongfeng] || StateList[Xinfengshuileng] || StateList[Xinfengqingxi]))
//                 step = 2;
//             else
//             {
//                 step = 0;
//             }
//         }
//         else if(step == 2)
//         {
//             if(StateList[NeedXinfengCool] == 1)
//             {
//                 if(StateList[Xinfengtongfeng] == 0)
//                     Xinfeng_Tongfeng(1);
//             }
//             else if(StateList[NeedXinfengCool] == 2)
//             {
//                 if(StateList[Xinfengshuileng] == 0)
//                     Xinfeng_Shuileng(1);
//             } 
//             step = 3;
//         }
//         else if(step == 3)
//         {
//             if(StateList[NeedXinfengCool] == 1)
//             {
//                 if(StateList[Xinfengtongfeng] == 1)
//                     step = 4;
//                 else 
//                     step = 2;
//             }
//             if(StateList[NeedXinfengCool] == 2)
//             {
//                 if(StateList[Xinfengshuileng] == 1)
//                     step = 4;
//                 else 
//                     step = 2;
//             }
//         }
*/
    
}
int ADPower = 0;
void ControlAction_AirCondition() 
{
    

    if(StateList[NeedADCool] == 0 && StateList[NeedDehumidity] == 0)
    {
//        if(ADPower == 1)
//        {
            Aircondition_AllOff(1);
            GPIO_SetBits(GPIOC,  GPIO_Pin_8);
            ADPower = 0;
//        }
    }
    else
    {
//        if(ADPower == 0)
//        {
            Aircondition_AllOn(1);
            GPIO_ResetBits(GPIOC,  GPIO_Pin_8);
            ADPower = 1;
//        }
    }
    if(ADPower && !(ErrorList[ADControlErr] || ErrorList[NoAD]))
        GPIO_ResetBits(LED3);
    else
        GPIO_SetBits(LED3);
}
extern u8 SearchFin;
void ControlTask()
{
    if(xinfeng15s)
    {
        xinfeng15s = 0;
        ControlAction_Xinfeng();
    }
    if(SearchFin == 1)
    {
        ControlAction_AirCondition();
    }
}

