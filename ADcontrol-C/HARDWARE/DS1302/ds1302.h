#ifndef _DS1302_H
#define _DS1302_H

#define DS1302_RST	    GPIOA,GPIO_Pin_6 
#define DS1302_IO		GPIOA,GPIO_Pin_5
#define DS1302_SCLK	    GPIOA,GPIO_Pin_4

void DS1302_Pin_Init(void);
void Time_Set(signed char* time);
void Time_Updata0(void);
void Time_Updata1(void);
#endif
