#include "12864.h"
#include "rs485.h"
#include "Show.h"
#include "delay.h"
#include "messagebox.h"
#include "ds1302.h"
#include "clock.h"
#include "stmflash.h"
#include "DME3000.h"
#include <stdio.h>

char shiwen1_s[]="08"; //室温
char shidu1_s[]=" 53 ";  //室内湿度
char shiwaiwendu[]="00";
char shiwen2_s[]="27"; //设定温度
char shidu2_s[]="  60";  //设定湿度
char qiangzhiqidong[]="05";  //空调强制启动温度
char gongzuotianshu_s[] = "00";
char zhouqi[]="030";  //清洗周期
char Addr_benji[] ="01"; //本机
char nian[]="16";
char yue[]="01";
char ri[]="27";
char shi[]="17";
char fen[]="57";
char miao[]="35";
char Baud_1[]=" 19200";
char Baud_2[]=" 19200";
char Baud_3[]=" 19200";
int Y_Offset = 0; //这个变量控制着屏幕的上下卷动
int Nowpage;      //表示现在在哪一个页面
StringStruct NowStruct;
StringStruct ErrBuf[7];
StringStruct lianjiekongtiao1_n,lianjiekongtiao2_n,lianjiekongtiao3_n,lianjiekongtiao4_n;
void FnOflianjiekongtiao1_n(int i);
//********************************
//字符串缓存的更新
//********************************
void NumToString(char *st,s32 num, u8 cnt)
{
    if(num>=0)
    {
        while(cnt--)
        {
            *(st+cnt) = (num % 10)+'0';
            num = num / 10;
        }
    }
    else 
    {
        *st = '-';
        cnt -= 1;
        while(cnt--)
        {
            *(st+cnt+1) = (num % 10)+'0';
            num = num / 10;
        }
    }
}
void StringUpdata(void)
{
    
    NumToString(gongzuotianshu_s, SetData[WorkDays], 2);
    NumToString(shiwen1_s, StateList[AverageTemp], 2);
    sprintf(shidu1_s," %d",StateList[AverageHumi]);
    //NumToString(shidu1_s, StateList[AverageHumi], 2);
    NumToString(shiwaiwendu, StateList[OutsideTemp], 2);
    NumToString(shiwen2_s, SetData[SetTemp], 2);
    sprintf(shidu2_s, " %d",SetData[LimitHumidity]);
    //NumToString(shidu2_s+1, SetData[LimitHumidity], 3);
    NumToString(qiangzhiqidong, SetData[LimitTemp], 2);
    NumToString(zhouqi, SetData[WashingInterval], 3);
    NumToString(Addr_benji, SetData[MyAddress], 2);
    NumToString(Baud_1+1, BaudRateList[SetData[BaudRate1]], 5);
    NumToString(Baud_2+1, BaudRateList[SetData[BaudRate2]], 5);
    NumToString(Baud_3+1, BaudRateList[SetData[BaudRate3]], 5);
    NumToString(nian, SetTime[0], 2);
    NumToString(yue, SetTime[1], 2);
    NumToString(ri, SetTime[2], 2);
    NumToString(shi, SetTime[3], 2);
    NumToString(fen, SetTime[4], 2);
    NumToString(miao, SetTime[5], 2);
    
}



//********************************
//首页的字码 页面编号1
//********************************
void FnOfshineiwendu1(int i);
char tempstringhome[] = "室内00℃室外00℃";
StringStruct shineiwendu1,shineiwendu_n1,shineishidu1,shineishidu_n1,shedingwendu1,shedingwendu_n1,shedingshidu1,shedingshidu_n1;
StringStruct shineiwendu1 = {
	1,7,1,tempstringhome,0,&shineishidu1,NULL,FnOfshineiwendu1
};
//StringStruct shineiwendu_n1 = {
//	6,6,1,shiwen1_s,0,&shineishidu1
//};
StringStruct shineishidu1 = {
	3,8,2,"湿度    %rh",0,&shineishidu_n1
};
StringStruct shineishidu_n1 = {
	5,6,2,shidu1_s,0,&shedingwendu1
};
StringStruct shedingwendu1 = {
	1,7,3,"设定室温    ℃",0,&shedingwendu_n1
};
StringStruct shedingwendu_n1 = {
	6,6,3,shiwen2_s,0,&shedingshidu1
};
StringStruct shedingshidu1 = {
	3,8,4,"湿度    %rh",0,&shedingshidu_n1
};
StringStruct shedingshidu_n1 = {
	5,6,4,shidu2_s,0
};
void FnOfshineiwendu1(int i)
{
    if(i==Key_Set)
    {
        PageJumpTo(2);
    }
}

//********************************
//设置页动作函数的声明
//********************************
void FnOflianjiekongtiao(int i);
void FnOfchakanggaojing(int i);
void FnOfshedingwendu(int i);
void FnOfzuigaoshidu(int i);
void FnOfkongtiaoqidongwendu(int i);
void FnOfqingxizhouqi(int i);
void FnOfshijianshezhi(int i);
void FnOfbenjidizhi(int i);
void FnOftongxinduankoushezhi(int i);
//********************************
//设置页的字码    页面编号2
//********************************
StringStruct lianjiekongtiao,chakanggaojing,shedingwendu,zuigaoshidu,kongtiaoqidongwendu,qingxizhouqi,shijianshezhi,benjidizhi,tongxinduankoushezhi;

StringStruct chakanggaojing = {
	1,6,1,"查看告警信息",1,&lianjiekongtiao,NULL,FnOfchakanggaojing
};
StringStruct lianjiekongtiao = {
    1,6,2,"查看所有空调",1,&shedingwendu,&chakanggaojing,FnOflianjiekongtiao
};
StringStruct shedingwendu = {
	1,4,3,"设定温度",1,&zuigaoshidu,&lianjiekongtiao,FnOfshedingwendu
};
StringStruct zuigaoshidu = {
	1,8,4,"设定最高允许湿度",1,&kongtiaoqidongwendu,&shedingwendu,FnOfzuigaoshidu
};
StringStruct kongtiaoqidongwendu = {
	1,8,5,"设定空调启动温度",1,&qingxizhouqi,&zuigaoshidu,FnOfkongtiaoqidongwendu
};
StringStruct qingxizhouqi = {
	1,6,6,"设定清洗周期",1,&shijianshezhi,&kongtiaoqidongwendu,FnOfqingxizhouqi
};
StringStruct shijianshezhi = {
	1,4,7,"时间设置",1,&benjidizhi,&qingxizhouqi,FnOfshijianshezhi
};
StringStruct benjidizhi = {
	1,6,8,"修改本机地址",1,&tongxinduankoushezhi,&shijianshezhi,FnOfbenjidizhi
};
StringStruct tongxinduankoushezhi = {
	1,6,9,"通信端口设置",1,NULL,&benjidizhi,FnOftongxinduankoushezhi
};

void FnOflianjiekongtiao(int i)
{
    if(i==Key_Set)
    {
         PageJumpTo(201);
    }
    if(i==Key_Return)
    {
        PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
}
void FnOfchakanggaojing(int i) //查看告警信息项的动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(20);
    }
    if(i==Key_Return)
    {
        PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
}
void FnOfshedingwendu(int i)//动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(21);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }    
}
void FnOfzuigaoshidu(int i)//动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(22);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    } 
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }    
}
void FnOfkongtiaoqidongwendu(int i)//动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(23);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }    
}
void FnOfqingxizhouqi(int i)//动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(24);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }    
}
void FnOfshijianshezhi(int i)//动作函数
{
    if(i==Key_Set)
    {
        CopyNowToSet();
         PageJumpTo(25);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }    
}
void FnOfbenjidizhi(int i)//动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(26);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }    
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
}
void FnOftongxinduankoushezhi(int i)//动作函数
{
    if(i==Key_Set)
    {
         PageJumpTo(27);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
}

//********************************
//设定温度页面    页面编号21
//********************************
void FnOfshedingwendu2_n(int i);
StringStruct shedingwendu2,shedingwendu2_n,shedingwendu21;
StringStruct shedingwendu2 = {
    3,6,1,"设定温度",0,&shedingwendu2_n,NULL
};
StringStruct shedingwendu2_n = {
    4,4,2,shiwen2_s,1,&shedingwendu21,&shedingwendu2,FnOfshedingwendu2_n
};
StringStruct shedingwendu21 = {
    5,5,2,"℃",0,NULL,&shedingwendu2_n
};

void FnOfshedingwendu2_n(int i)
{
    if(i==Key_Return)
    {
         Aircondition_SetTempAll(1);
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetData[SetTemp]++;
        PageUpdata();
				
    }   
    if(i==Key_Down)
    {
        SetData[SetTemp]--;
        PageUpdata();
				
    }    
		
}
//********************************
//设定最高允许湿度页面    页面编号22
//********************************
void FnOfshedingshidu2_n(int i);
StringStruct shedingshidu2,shedingshidu2_n,shedingshidu21;
StringStruct shedingshidu2 = {
    2,7,1,"最高允许湿度",0,&shedingshidu2_n,NULL
};
StringStruct shedingshidu2_n = {
    3,4,2,shidu2_s,1,&shedingshidu21,&shedingshidu2,FnOfshedingshidu2_n
};
StringStruct shedingshidu21 = {
    5,6,2," %rh",0,NULL,&shedingshidu2_n
};
void FnOfshedingshidu2_n(int i)
{
    if(i==Key_Return)
    {
        Aircondition_SetHumAll(1);
        PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetData[LimitHumidity]++;
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetData[LimitHumidity]--;
        PageUpdata();
    }        
}

//********************************
//设定空调启动温度页面    页面编号23
//********************************
void FnOfkongtiaoqidong1_n(int i);
StringStruct kongtiaoqidong1,kongtiaoqidong1_n,kongtiaoqidong11,kongtiaoqidong12;
StringStruct kongtiaoqidong1 = {
    1,8,1,"室温高于设定温度",0,&kongtiaoqidong11,NULL
};
StringStruct kongtiaoqidong11 = {
    5,6,2,"℃时",0,&kongtiaoqidong1_n,&kongtiaoqidong1
};
StringStruct kongtiaoqidong1_n = {
    4,4,2,qiangzhiqidong,1,&kongtiaoqidong12,&kongtiaoqidong11,FnOfkongtiaoqidong1_n
};
StringStruct kongtiaoqidong12 = {
    2,7,3,"强制启动空调",0,NULL,&kongtiaoqidong1_n
};
void FnOfkongtiaoqidong1_n(int i)
{
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetData[LimitTemp]++;
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetData[LimitTemp]--;
        PageUpdata();
    }        
}

//********************************
//设定清洗周期    页面编号24
//********************************
void FnOfqingxizhouqi1_n(int i);
StringStruct qingxizhouqi1,qingxizhouqi1_n,qingxizhouqi11;
StringStruct qingxizhouqi1 = {
    1,8,1,"水帘系统清洗周期",0,&qingxizhouqi11,NULL
};
StringStruct qingxizhouqi11 = {
    6,6,2,"天",0,&qingxizhouqi1_n,&qingxizhouqi1
};
StringStruct qingxizhouqi1_n = {
    4,5,2,zhouqi,0,NULL,&qingxizhouqi11,FnOfqingxizhouqi1_n
};
void FnOfqingxizhouqi1_n(int i)
{
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetData[WashingInterval]++;
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetData[WashingInterval]--;
        PageUpdata();
    }        
}

//********************************
//设置时间    页面编号25
//********************************
void FnOfshijianshezhi1nian(int i);
void FnOfshijianshezhi1yue(int i);
void FnOfshijianshezhi1ri(int i);
void FnOfshijianshezhi1shi(int i);
void FnOfshijianshezhi1fen(int i);
void FnOfshijianshezhi1miao(int i);
StringStruct shijianshezhi1,shijianshezhi11,shijianshezhi12,shijianshezhi1nian,shijianshezhi1yue,shijianshezhi1ri,shijianshezhi1shi,shijianshezhi1fen,shijianshezhi1miao;
StringStruct shijianshezhi1 = {
    3,6,1,"设置时间",0,&shijianshezhi11,NULL
};
StringStruct shijianshezhi11 = {
    2,7,2,"  年  月  日",0,&shijianshezhi12,&shijianshezhi1
};
StringStruct shijianshezhi12 = {
    2,7,3,"  时  分  秒",0,&shijianshezhi1nian,&shijianshezhi11
};
StringStruct shijianshezhi1nian = {
    2,2,2,nian,1,&shijianshezhi1yue,&shijianshezhi12,FnOfshijianshezhi1nian
};
StringStruct shijianshezhi1yue = {
    4,4,2,yue,1,&shijianshezhi1ri,&shijianshezhi1nian,FnOfshijianshezhi1yue
};
StringStruct shijianshezhi1ri = {
    6,6,2,ri,1,&shijianshezhi1shi,&shijianshezhi1yue,FnOfshijianshezhi1ri
};
StringStruct shijianshezhi1shi = {
    2,2,3,shi,1,&shijianshezhi1fen,&shijianshezhi1ri,FnOfshijianshezhi1shi
};
StringStruct shijianshezhi1fen = {
    4,4,3,fen,1,&shijianshezhi1miao,&shijianshezhi1shi,FnOfshijianshezhi1fen
};
StringStruct shijianshezhi1miao = {
    6,6,3,miao,1,NULL,&shijianshezhi1fen,FnOfshijianshezhi1miao
};

void SaveTimeIntoDS1302()
{
    
}
void FnOfshijianshezhi1nian(int i)
{
    if(i==Key_Set)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetTime[0]++;
        shijianyuejie1(SetTime);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetTime[0]--;
        shijianyuejie1(SetTime);
        PageUpdata();
    }       
}
void FnOfshijianshezhi1yue(int i)
{
     if(i==Key_Set)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetTime[1]++;
        shijianyuejie1(SetTime);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetTime[1]--;
        shijianyuejie1(SetTime);
        PageUpdata();
    }       
}
void FnOfshijianshezhi1ri(int i)
{
    if(i==Key_Set)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetTime[2]++;
        shijianyuejie1(SetTime);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetTime[2]--;
        shijianyuejie1(SetTime);
        PageUpdata();
    }       
}
void FnOfshijianshezhi1shi(int i)
{
    if(i==Key_Set)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetTime[3]++;
        shijianyuejie1(SetTime);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetTime[3]--;
        shijianyuejie1(SetTime);
        PageUpdata();
    }       
}
void FnOfshijianshezhi1fen(int i)
{
    if(i==Key_Set)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetTime[4]++;
        shijianyuejie1(SetTime);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetTime[4]--;
        shijianyuejie1(SetTime);
        PageUpdata();
    }       
}
void FnOfshijianshezhi1miao(int i)
{
    if(i==Key_Set)
    {
        SaveSetTime();
        PageJumpTo(1);
    }
    if(i==Key_Return)
    {
        PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetTime[5]++;
        shijianyuejie1(SetTime);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetTime[5]--;
        shijianyuejie1(SetTime);
        PageUpdata();
    }       
}
//********************************
//修改本机地址 页面编号26
//********************************
void FnOfxiugaidizhi1_n(int i);
StringStruct xiugaidizhi1,xiugaidizhi1_n,xiugaidizhi11;
StringStruct xiugaidizhi1 = {
    1,8,1,"修改本机通讯地址",0,&xiugaidizhi1_n,NULL
};
StringStruct xiugaidizhi1_n = {
    6,6,3,Addr_benji,0,&xiugaidizhi11,&xiugaidizhi1,FnOfxiugaidizhi1_n
};
StringStruct xiugaidizhi11 = {
    2,5,3,"地址号:",0,NULL,&xiugaidizhi1_n
};
void FnOfxiugaidizhi1_n(int i)
{
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        SetData[MyAddress]++;
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        SetData[MyAddress]--;
        PageUpdata();
    }       
}

//***********************************************
//通信端口设置    页面编号27
//***********************************************
void FnOftongxinduankoushezhi11_n(int i);
void FnOftongxinduankoushezhi12_n(int i);
void FnOftongxinduankoushezhi13_n(int i);
StringStruct tongxinduankoushezhi1,tongxinduankoushezhi11,tongxinduankoushezhi12,tongxinduankoushezhi13,tongxinduankoushezhi11_n,tongxinduankoushezhi12_n,tongxinduankoushezhi13_n;
StringStruct tongxinduankoushezhi1 = {
    2,7,1,"通信端口设置",0,&tongxinduankoushezhi11,NULL
};
StringStruct tongxinduankoushezhi11 = {
    1,4,2,"空调通信",0,&tongxinduankoushezhi12,&tongxinduankoushezhi1
};
StringStruct tongxinduankoushezhi12 = {
    1,4,3,"新风通信",0,&tongxinduankoushezhi13,&tongxinduankoushezhi11
};
StringStruct tongxinduankoushezhi13 = {
    1,4,4,"近端通信",0,&tongxinduankoushezhi11_n,&tongxinduankoushezhi12
};
StringStruct tongxinduankoushezhi11_n = {
    5,8,2,Baud_1,1,&tongxinduankoushezhi12_n,&tongxinduankoushezhi13,FnOftongxinduankoushezhi11_n
};
StringStruct tongxinduankoushezhi12_n = {
    5,8,3,Baud_2,1,&tongxinduankoushezhi13_n,&tongxinduankoushezhi11_n,FnOftongxinduankoushezhi12_n
};
StringStruct tongxinduankoushezhi13_n = {
    5,8,4,Baud_3,1,NULL,&tongxinduankoushezhi12_n,FnOftongxinduankoushezhi13_n
};

void FnOftongxinduankoushezhi11_n(int i)
{
    if(i==Key_Set)
    {
        SetData[BaudRate1]--;
        PageUpdata();
        USART_Initialize(1, BaudRateList[SetData[BaudRate1]]);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }       
}
void FnOftongxinduankoushezhi12_n(int i)
{
    if(i==Key_Set)
    {
        SetData[BaudRate2]--;
        PageUpdata();
        USART_Initialize(1, BaudRateList[SetData[BaudRate2]]);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }       
}
void FnOftongxinduankoushezhi13_n(int i)
{
    if(i==Key_Set)
    {
        SetData[BaudRate3]--;
        PageUpdata();
        USART_Initialize(1, BaudRateList[SetData[BaudRate3]]);
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }       
}



//***********************************************
//显示整个链表
//***********************************************
void Display_OnePage(StringStruct start)
{
	StringStruct* p = &start;
    LCD_Initinal1();
	do
	{
        Lcd_Display(p->X, p->Y - Y_Offset, p->String);
		p = p->next;
	}while(p != NULL);


}

//***********************************************
//显示一条
//***********************************************
void Display_OneStr(StringStruct st)
{
	Lcd_Display(st.X, st.Y - Y_Offset, st.String);
}

//***********************************************
//反白显示一条
//***********************************************
void Display_Chosed(StringStruct st)
{
    ClearGraphic();
	Set_White(st.X, st.Y - Y_Offset, st.X_End, 0);
}
//***********************************************
//从链表中选择下一个可选的结构体
//***********************************************
StringStruct ChosedNext(StringStruct st)
{
    StringStruct* p = st.next;    
    do
    {
        if(p == NULL) return st;
        else if(p->ChoseEn)
        {
            if(p->Y - Y_Offset >4) Y_Offset++;
            if(Y_Offset>4) Y_Offset=4;
            return *p;
        }
        else p = p->next;
    }while(1);
}
//***********************************************
//从链表中选择上一个可选的结构体
//***********************************************
StringStruct ChosedLast(StringStruct st)
{
    StringStruct* p = st.last;    
    do
    {
        if(p == NULL) return st;
        else if(p->ChoseEn)
        {
            if(p->Y - Y_Offset <1) Y_Offset--;
            if(Y_Offset<0) Y_Offset=0;
            return *p;
        }
        else p = p->last;
    }while(1);
}
//***********************************************
//让一个字符串闪起来
//***********************************************
void Display_Flash(StringStruct st)
{
	static unsigned int time;
	if (time)
	{
		Set_White(st.X, st.Y - Y_Offset, st.X_End, 1);
		time = 0;
	}
	else
	{
		Set_White(st.X, st.Y - Y_Offset, st.X_End, 0);
		time = 1;
	}

}
//***********************************************
//刷新页面
//***********************************************
void PageUpdata(void)
{
    yuejie();
    StringUpdata();
    if(Nowpage == 1) 
    {
        sprintf(tempstringhome, "室内%2d℃室外%2d℃", StateList[AverageTemp], StateList[OutsideTemp]);
        Display_OnePage(shineiwendu1);
    }
    if(Nowpage == 2)
    {
        Display_OnePage(chakanggaojing);
        Display_Chosed(NowStruct);
    }
    if(Nowpage == 20)
    {
        Err_Updata();
        Display_OnePage(ErrBuf[0]);
        Display_Chosed(NowStruct);
    }
    if(Nowpage == 21)
    {
        Display_OnePage(shedingwendu2);
        Display_Chosed(NowStruct);
    }
    if(Nowpage == 22)
    {
        Display_OnePage(shedingshidu2);
        Display_Chosed(NowStruct);        
    }
    if(Nowpage == 23)
    {
        Display_OnePage(kongtiaoqidong1);
        Display_Chosed(NowStruct);        
    }
    if(Nowpage == 24)
    {
        Display_OnePage(qingxizhouqi1);
        Display_Chosed(NowStruct);        
    }
    if(Nowpage == 25)
    {
        Display_OnePage(shijianshezhi1);
        Display_Chosed(NowStruct);        
    }
    if(Nowpage == 26)
    {
        Display_OnePage(xiugaidizhi1);
        Display_Chosed(NowStruct);        
    }
    if(Nowpage == 27)
    {
        Display_OnePage(tongxinduankoushezhi1);
        Display_Chosed(NowStruct);        
    }
    if(Nowpage == 201)
    {
        Display_OnePage(lianjiekongtiao1_n);
    }
}
//***********************************************
//跳转到指定页面
//***********************************************
void PageJumpTo(int pagenum)
{
	if(pagenum == 1) //home
    {
        
        Y_Offset = 0;
        Nowpage = 1;
        NowStruct = shineiwendu1;
		STMFLASH_Write(FLASH_SAVE_ADDR, (u32*)SetData, sizeof(SetData)/4);
        PageUpdata();
    }
    else if(pagenum == 2) // setting
    {
        Y_Offset = 0;
        Nowpage = 2;
        NowStruct = chakanggaojing;
        PageUpdata();

    }
    else if(pagenum == 20) //设定温度页面
    {
        Y_Offset = 0;
        Nowpage = 20;
        NowStruct = ErrBuf[0];
        PageUpdata();
    }
    else if(pagenum == 21) //设定温度页面
    {
        Y_Offset = 0;
        Nowpage = 21;
        NowStruct = shedingwendu2_n;
        PageUpdata();
    }
    else if(pagenum == 22) //设定湿度页面
    {
        Y_Offset = 0;
        Nowpage = 22;
        NowStruct = shedingshidu2_n;
        PageUpdata();
    }
    else if(pagenum == 23) //空调强制启动值
    {
        Y_Offset = 0;
        Nowpage = 23;
        NowStruct = kongtiaoqidong1_n;
        PageUpdata();
    }
    else if(pagenum == 24) //水帘系统清洗周期
    {
        Y_Offset = 0;
        Nowpage = 24;
        NowStruct = qingxizhouqi1_n;
        PageUpdata();
    }
    else if(pagenum == 25) //设置本机通讯地址
    {
        Y_Offset = 0;
        Nowpage = 25;
        NowStruct = shijianshezhi1nian;
        PageUpdata();
    }
    else if(pagenum == 26) //设置本机通讯地址
    {
        Y_Offset = 0;
        Nowpage = 26;
        NowStruct = xiugaidizhi1_n;
        PageUpdata();
    }
    else if(pagenum == 27) //设置本机通讯地址
    {
        Y_Offset = 0;
        Nowpage = 27;
        NowStruct = tongxinduankoushezhi11_n;
        PageUpdata();
    }
    else if(pagenum == 201) //设置查看所有空调
    {
        Y_Offset = 0;
        Nowpage = 201;
        NowStruct = lianjiekongtiao1_n;
        FnOflianjiekongtiao1_n(Key_Up);
        PageUpdata();
    }
}

//********************************
//查看所有空调 页面编号201
//********************************
//StringStruct lianjiekongtiao1_n,lianjiekongtiao2_n,lianjiekongtiao3_n,lianjiekongtiao4_n;
char string01[] = "工作状态  关机  ";
char string02[] = "室温00℃外温00℃";
char string03[] = "设定温湿00℃00% ";
char string04[] = "地址000 湿度00% ";
StringStruct lianjiekongtiao1_n = {
    1,8,4,string01,0,&lianjiekongtiao2_n,NULL,FnOflianjiekongtiao1_n
};
StringStruct lianjiekongtiao2_n = {
    1,8,2,string02,0,&lianjiekongtiao3_n,NULL
};
StringStruct lianjiekongtiao3_n = {
    1,8,3,string03,0,&lianjiekongtiao4_n,NULL
};
StringStruct lianjiekongtiao4_n = {
    1,8,1,string04,0,NULL,NULL
};
void FnOflianjiekongtiao1_n(int i)
{
    static int j=0,k=0;
    if(i==Key_Return)
    {
         PageJumpTo(1);
    }
    if(i==Key_Up)
    {
        for(k=0; k<256; k++)
        {
            j++;
            if(j>255) j=0;
            if(ADList[j].EN == 1)
            {
                sprintf(lianjiekongtiao4_n.String,"地址%03d 湿度%02drh", j,ADList[j].humi);
                sprintf(lianjiekongtiao2_n.String,"室温%02d℃外温%02d℃", ADList[j].tempInside,ADList[j].tempOutside);
                sprintf(lianjiekongtiao3_n.String,"设定温湿%02d℃%02drh", ADList[j].settingTemp,ADList[j].settingHumi);
                if(ADList[j].State)
                {
                    sprintf(lianjiekongtiao1_n.String,"工作状态  开机  ");
                }
                else
                {
                    sprintf(lianjiekongtiao1_n.String,"工作状态  关机  ");
                }
                break;
            }

        }
        if(k==256)
        {
                sprintf(lianjiekongtiao4_n.String,"                ");
                sprintf(lianjiekongtiao2_n.String,"  检测不到空调  ");
                sprintf(lianjiekongtiao3_n.String,"                ");
                sprintf(lianjiekongtiao1_n.String,"                ");
        }
        PageUpdata();
    }   
    if(i==Key_Down)
    {
        for(k=0; k<256; k++)
        {
            j--;
            if(j<0) j=255;
            if(ADList[j].EN == 1)
            {
                sprintf(lianjiekongtiao4_n.String,"地址%03d 湿度%02drh", j,ADList[j].humi);
                sprintf(lianjiekongtiao2_n.String,"室温%02d℃外温%02d℃", ADList[j].tempInside,ADList[j].tempOutside);
                sprintf(lianjiekongtiao3_n.String,"设定温湿%02d℃%02drh", ADList[j].settingTemp,ADList[j].settingHumi);
                if(ADList[j].State)
                {
                    sprintf(lianjiekongtiao1_n.String,"工作状态  开机  ");
                }
                else
                {
                    sprintf(lianjiekongtiao1_n.String,"工作状态  关机  ");
                }
                break;
            }

            
        }
        if(k==256)
        {
                sprintf(lianjiekongtiao4_n.String,"                ");
                sprintf(lianjiekongtiao2_n.String,"  检测不到空调  ");
                sprintf(lianjiekongtiao3_n.String,"                ");
                sprintf(lianjiekongtiao1_n.String,"                ");
        }
        PageUpdata();
    }    
}
//********************************
//查看警告页面  页面编号20
//********************************

char errstring1[] = "新风系统控制故障";
char errstring2[] = "空调控制故障";
char errstring3[] = "除湿故障";
char errstring4[] = "新风系统效率低";
char errstring5[] = "取温度失败";
char errstring6[] = "缺水故障";
char errstring7[] = "    工作正常";
char errstring8[] = "未检测到空调";

void FnOfErrPage(int i)
{
    if(i==Key_Set)
    {
        
    }
    if(i==Key_Return)
    {
         PageJumpTo(1);
    } 
    if(i==Key_Up)
    {
        NowStruct = ChosedLast(NowStruct);
        PageUpdata();
    }
    if(i==Key_Down)
    {
        NowStruct = ChosedNext(NowStruct);
        PageUpdata();
    }    
}

int Err_Init()
{
    StringStruct* p = ErrBuf;
    int i;
    p = ErrBuf;
    p->ChoseEn = 1;
    p->next = p+1;
    p->last = NULL;
    p->String = NULL;
    p->X = 1;
    p->X_End = 8;
    p->Y = 1;
    p->Switch = FnOfErrPage;
    for(i=1; i<5; i++)
    {
        p = ErrBuf +i;
        p->ChoseEn = 1;
        p->next = p+1;
        p->last = p-1;
        p->String = NULL;
        p->X = 1;
        p->X_End = 8;
        p->Y = i+1;
        p->Switch = FnOfErrPage;
        
    }
    p = ErrBuf +5;
    p->ChoseEn = 1;
    p->next = NULL;
    p->last = p-1;
    p->String = NULL;
    p->X = 1;
    p->X_End = 8;
    p->Y = 6;
    p->Switch = FnOfErrPage;
    return 0;
}
/*
int Err_Add(char* errstring)
{
    StringStruct* p_err = ErrBuf;
    while(p_err->next != NULL)
    {
        if(p_err->String == errstring) return -1;
        p_err = p_err->next;
    }
    p_err->next = p_err+1;
    ((StringStruct*)p_err->next)->last = p_err;
    ((StringStruct*)p_err->next)->String = errstring;
    ((StringStruct*)p_err->next)->Y = p_err->Y + 1;
    return 0;
}
int Err_Del(char* errstring)
{
    StringStruct* p_err = ErrBuf;
    int i;

    for(i=0; i<10; i++)
    {
        if(p_err->String == errstring) break;
        p_err = p_err->next;
    }
    if(p_err->String != errstring) return -1;
    while((p_err+1)->String != NULL)
    {
        p_err->String = ((StringStruct*)p_err->next)->String;
        p_err->next = p_err+1;
    ((StringStruct*)p_err->next)->last = p_err;
    ((StringStruct*)p_err->next)->String = errstring;
    ((StringStruct*)p_err->next)->Y = p_err->Y + 1;
    }
    return 0;
}
*/
void Err_Updata(void)
{
    int NumOfErr = 0;
    NumOfErr = 0;
    if(ErrorList[XFSLControlErr] == 1)
    {
        ErrBuf[NumOfErr].String = errstring1;
        NumOfErr++;
    }
    if(ErrorList[ADControlErr] == 1)
    {
        ErrBuf[NumOfErr].String = errstring2;
        NumOfErr++;
    }
    if(ErrorList[ShortOfWater] == 1)
    {
        ErrBuf[NumOfErr].String = errstring6;
        NumOfErr++;
    }
    if(ErrorList[LowEfficiency] == 1)
    {
        ErrBuf[NumOfErr].String = errstring4;
        NumOfErr++;
    }
    if(ErrorList[GetTempErr] == 1)
    {
        ErrBuf[NumOfErr].String = errstring5;
        NumOfErr++;
    }
    if(ErrorList[DeHumiErr] == 1)
    {
        ErrBuf[NumOfErr].String = errstring3;
        NumOfErr++;
    }
    if(ErrorList[NoAD] == 1)
    {
        ErrBuf[NumOfErr].String = errstring8;
        NumOfErr++;
    }
    if(NumOfErr == 0)
    {
        ErrBuf[NumOfErr].String = errstring7;
        NumOfErr++;
    }

    while(NumOfErr < 7)
    {
        ErrBuf[NumOfErr].String = NULL;
        NumOfErr++;
    }
}
