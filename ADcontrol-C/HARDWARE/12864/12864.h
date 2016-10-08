#ifndef _12864_h
#define _12864_h



#define RS  GPIOC,GPIO_Pin_12
#define WRD GPIOC,GPIO_Pin_11
#define E	GPIOC,GPIO_Pin_10

#define DATAPROT	GPIOD

#include "stm32f4xx.h"

void LCD_PortInit(void);
void LCD_Initinal(void);
void LCD_Initinal1(void);
void TransferData(char data1, u8 DI);
void display(void);
void display_grapic(void);
void delayms(u16 n);
void DisplayLine(u8 line1, u8 line2);
void DisplayGraphic(u8 const *adder);
void delay(u16 m);
void lcd_mesg(char* adder1);
void Lcd_Display(unsigned char x, unsigned char y, char *address);
void Display_Cursor_Line(unsigned char line);
void Set_White(u8 x,u8 y,u8 end_x,u8 clear);
void ClearGraphic(void);
#endif // !

