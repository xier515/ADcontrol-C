#include "12864.h"
#include "delay.h"


void LCD_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12);
    
    GPIO_InitStructure.GPIO_Pin = 0x00ff;   //运算结果是GPIO_PIN_0_7
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOC, 0x00ff);
}

/*void LCD_Initinal1(void)           //LCD字库初始化程序
{
	LCD_PortInit();
	delay(40);             //大于40MS的延时程序
//	PSB = 1;                 //设置为8BIT并口工作模式 //硬件拉高
	delay(1);              //延时
	delay(10);
	TransferData(0x30, 0);  //Extended Function Set :8BIT设置,RE=0: basic instruction set, G=0 :graphic display OFF
	delay(100);            //大于100uS的延时程序
	TransferData(0x30, 0);  //Function Set
	delay(37);             ////大于37uS的延时程序
//	TransferData(0x08, 0);  //Display on Control
    TransferData(0x08, 1);  //Display on Control
	delay(100);            //大于100uS的延时程序
	TransferData(0x10, 0);  //Cursor Display Control光标设置
	delay(100);            //大于100uS的延时程序
	TransferData(0x0C, 0);  //Display Control,D=1,显示开
	delay(100);            //大于100uS的延时程序
	TransferData(0x01, 0);  //Display Clear
	delayms(1);             //大于10mS的延时程序
	TransferData(0x06, 0);  //Enry Mode Set,光标从右向左加1位移动
	delay(100);            //大于100uS的延时程序
}
*/
void LCD_Initinal1(void)           //LCD字库初始化程序
{
	delay(4);             //大于40MS的延时程序
//	PSB = 1;                 //设置为8BIT并口工作模式 //硬件拉高
	delay(1);              //延时
	delay(10);
	TransferData(0x30, 0);  //Extended Function Set :8BIT设置,RE=0: basic instruction set, G=0 :graphic display OFF
	delay(10);            //大于100uS的延时程序
	TransferData(0x30, 0);  //功能设置 8位数据，基本指令
	delay(4);             ////大于37uS的延时程序
//	TransferData(0x08, 0);  //Display on Control
    TransferData(0x0C, 0);  //显示状态 ON，游标OFF，反白OFF
	delay(10);            //大于100uS的延时程序
//	TransferData(0x11, 0);  //光标向右
	delay(10);            //大于100uS的延时程序
	TransferData(0x0C, 0);  //显示状态 ON，游标OFF，反白OFF
	delay(10);            //大于100uS的延时程序
    TransferData(0x02, 0);  //光标归位
	delayms(1);             //大于10mS的延时程序
	TransferData(0x01, 0);  //Display Clear
	delayms(1);             //大于10mS的延时程序
	TransferData(0x06, 0);  //Enry Mode Set,光标右加1位移动
	delay(10);            //大于100uS的延时程序
}

void LCD_Initinal(void)           //LCD字库初始化程序
{
//	LCD_PortInit();
	delay(40);             //大于40MS的延时程序
//	PSB = 1;                 //设置为8BIT并口工作模式 //硬件拉高
	delay(1);              //延时
	delay(10);
    TransferData(0x36,0);  //Extended Function Set RE=1: extended instruction
    delay(10);            //大于100uS的延时程序
    TransferData(0x36,0);  //Extended Function Set:RE=1: extended instruction set
    delay(4);             ////大于37uS的延时程序
    TransferData(0x3E,0);  //EXFUNCTION(DL=8BITS,RE=1,G=1)
    delay(10);            //大于100uS的延时程序
//    TransferData(0x01,0);  //CLEAR SCREEN
//    delayms(1);            //大于100uS的延时程序

}


void   lcd_mesg( char  *adder1)
{
	unsigned char i;
	TransferData(0x80, 0);  //Set Graphic Display RAM Address
	delay(100);
	for (i = 0; i<32; i++)
	{
		TransferData(*adder1, 1);
		adder1++;

	}

	TransferData(0x90, 0);  //Set Graphic Display RAM Address
	delay(100);

	for (i = 32; i<64; i++)
	{
		TransferData(*adder1, 1);
		adder1++;
	}
    TransferData(0x02, 0);  
	delay(100);            //大于100uS的延时程序
}
      
//x must be less than 8 and y must be less than 4
void Lcd_Display(unsigned char x, unsigned char y, char *address)
{
    if(y<1 || y>4) return;
    if(address == 0) return;
    delay(5);
    switch(y)
    {
        case 1:TransferData(0x80+x-1, 0);break;
        case 2:TransferData(0x90+x-1, 0);break;
        case 3:TransferData(0x88+x-1, 0);break;
        case 4:TransferData(0x98+x-1, 0);break;
    }
    delay(5);
    while(*address != '\0')
    {
        TransferData(*address, 1);
		address++;
    }
}


void    TransferData(char data1, u8 DI)  //传送数据或者命令,当DI=0是,传送命令,当DI=1,传送数据.
{
    u16 buf;
    
	GPIO_ResetBits(WRD);
	
    if(DI&0x01)
        GPIO_SetBits(RS);
    else
        GPIO_ResetBits(RS);
	delay(4);
    buf = GPIO_ReadInputData(DATAPROT);
    buf &= 0xff00;
    buf |= data1;
	GPIO_Write(DATAPROT, buf);
	GPIO_SetBits(E);
	delay(4);
	GPIO_ResetBits(E);
    delay(4);
}


void    delayms(u16 n)            //延时10×n毫秒程序
{
	delay_ms(10*n);
}


void    delay(u16 m)            //延时程序
{
	delay_us(10*m);
}

void DisplayGraphic( unsigned char const *adder)
{

   int i,j;
//*******显示上半屏内容设置
   for(i=0;i<32;i++)              //
    { 
      TransferData((0x80 + i),0); //SET  垂直地址 VERTICAL ADD
      TransferData(0x80,0);       //SET  水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         TransferData(*adder,1);
         adder++;
       }
    }	   
//*******显示下半屏内容设置
   for(i=0;i<32;i++)              //
    {
      TransferData((0x80 + i),0); //SET 垂直地址 VERTICAL ADD
      TransferData(0x88,0);       //SET 水平地址 HORIZONTAL ADD
      for(j=0;j<16;j++)
       {
         TransferData(*adder,1);
         adder++;
       }
    }
}

//void DisplayChar(unsigned char

void Set_Draw(void)
{
	//W_1byte(0,0,0x01);			//清屏
	delay(20);					//延时
	//W_1byte(0,0,0x34);			//8BIT控制界面，扩充指令集,，绘图显示OFF
//	TransferData(0x34, 0);
    TransferData(0x34, 0);
	delay(20);					//延时
}

/***************************************************/
//反白函数
/**************************************************/
void Set_White(u8 x,u8 y,u8 end_x,u8 clear)
{
 	u8 i, j, white_x, white_y,white_end_x;//,clr_x,clr_y;		//
    if(y<1 || y>4) return;
    LCD_Initinal();
    Set_Draw();
    ClearGraphic();
    if(x <= end_x)
    {
        white_end_x = (end_x-x+1);
        white_end_x <<= 1;
        if(y==1)
        {
            white_x = (0x80+x-1);
            white_y = 0x80;
 //           clr_x = 0x80;
 //           clr_y = 0x80;
        }
        else if(y==2)
        {
            white_x = (0x80+x-1);
            white_y = 0x90;
 //           clr_x = 0x80;
 //           clr_y = 0x90;
        }
        else if(y==3)
        {
            white_x = (0x88+x-1);
            white_y = 0x80;
  //          clr_x = 0x88;
  //          clr_y = 0x80;
        }
        else if(y==4)
        {
            white_x = (0x88+x-1);
            white_y = 0x90;
  //          clr_x = 0x88;
//            clr_y = 0x90;
        }
//        Set_Draw();
/*        
        if(clear==0)//要反白时，先将整行的液晶全部清成不反白（此处行指y）
        {
            for(i=0;i<16;i++ )			//16行
            {
                TransferData(clr_y++, 0);	//设置绘图区的Y地址坐标0
                TransferData(clr_x, 0);		//设置绘图区的X地址坐标0
//                delay(10);
                for(j=0;j<16;j++)		//
                {
                    TransferData(0x00,1);	//清成不反白                    
                    delay(10);
                }
            }
        
        }
*/
//        delay(10);
        for(i=0;i<16;i++ )				//16行，因为是16*16汉字
        {
            TransferData(white_y++,0);		//设置绘图区的Y地址坐标0
            TransferData(white_x, 0);		//设置绘图区的X地址坐标0
//            delay(5);
            for(j=0;j<white_end_x;j++)	//
            {
                 if(clear==1)
                 {
                    TransferData(0x00, 1); //取消这一行的8个点的反白，液晶地址自动加1
                                       //（此处行指一个一个液晶点所组成的行）
                 }
                 else
                 {
                    TransferData(0xff, 1); //反白这一行的8个点，液晶地址自动加1
                                       //（此处行指一个一个液晶点所组成的行）
                 }
 //                delay(5);
            }
        }
        TransferData(0x36, 0);
//        delay(5);
        
    }
}
/*************************************************/

void ClearGraphic()
{

   int i,j;
    Set_Draw();
//*******显示上半屏内容设置
   for(i=0;i<32;i++)              //
    { 
        TransferData((0x80 + i),0); //SET  垂直地址 VERTICAL ADD
        TransferData(0x80,0);       //SET  水平地址 HORIZONTAL ADD
        delay(8);  
        for(j=0;j<16;j++)
        {
            TransferData(0x00,1);
//            delay(2); 
         
        }
    }	   
//*******显示下半屏内容设置
   for(i=0;i<32;i++)              //
    {
        TransferData((0x80 + i),0); //SET 垂直地址 VERTICAL ADD
        TransferData(0x88,0);       //SET 水平地址 HORIZONTAL ADD
        delay(8);  
        for(j=0;j<16;j++)
        {
            TransferData(0x00,1);
//           delay(2); 
         
        }
    }
    TransferData(0x36, 0);
//    delay(5);
}

