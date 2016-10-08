#include "stm32f4xx.h"
#include "delay.h"

static u8  fac_us=0;								   
static u16 fac_ms=0;							


void delay_init(unsigned char SYSCLK)
{


 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8); 
	fac_us=SYSCLK/8;						
	fac_ms=(u16)fac_us*1000;				

}								    


void delay_us(unsigned int nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 				
	SysTick->VAL=0x00;        				
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; 
	SysTick->VAL =0X00;       				
}

void delay_xms(unsigned short nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;			
	SysTick->VAL =0x00;           			
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       
	SysTick->VAL =0X00;     		  			  	    
} 

void delay_ms(unsigned short nms)
{	 	 
	u8 repeat=nms/540;						
											
	u16 remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 

			 



































