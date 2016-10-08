//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  SHTC1 Sample Code (V1.0)
// File      :  i2c_hal.c (V1.0)
// Author    :  RFU
// Date      :  26-Nov-2012
// Controller:  STM32F100RB
// IDE       :  µVision V4.60.0.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer
//==============================================================================

//-- Includes ------------------------------------------------------------------
#include "i2c_hal.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "delay.h"

unsigned char I2C_Num;

void DelayMicroSeconds(unsigned int nms)
{
    delay_us(nms);
}
void SDA_LOW()
{
    if(I2C_Num == 1)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_7);
    }
    else if(I2C_Num == 2)
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_9);
    }
}
void SDA_OPEN()
{
    if(I2C_Num == 1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_7);
    }
    else if(I2C_Num == 2)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_9);
    }
}
u8t SDA_READ()
{
    if(I2C_Num == 1)
    {
        return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
    }
    else if(I2C_Num == 2)
    {
        return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);
    }
    return 1;
}
void SCL_LOW()
{
    if(I2C_Num == 1)
    {
        GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    }
    else if(I2C_Num == 2)
    {
        GPIO_ResetBits(GPIOA,GPIO_Pin_8);
    }
}
void SCL_OPEN()
{
    if(I2C_Num == 1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_6);
    }
    else if(I2C_Num == 2)
    {
        GPIO_SetBits(GPIOA,GPIO_Pin_8);
    }
}
u8t SCL_READ()
{
    if(I2C_Num == 1)
    {
        return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6);
    }
    else if(I2C_Num == 2)
    {
        return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
    }
    return 1;
}
//==============================================================================
void I2c_Init(void){                      /* -- adapt the init for your uC -- */
//==============================================================================
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  
    
    I2C_Num = 1;
    SDA_OPEN();                  // I2C-bus idle mode SDA released
    SCL_OPEN();                  // I2C-bus idle mode SCL released

    I2C_Num = 2;
    SDA_OPEN();                  // I2C-bus idle mode SDA released
    SCL_OPEN();                  // I2C-bus idle mode SCL released
}

//==============================================================================
void I2c_StartCondition(void){
//==============================================================================
  SDA_OPEN();
  DelayMicroSeconds(1);
  SCL_OPEN();
  DelayMicroSeconds(1);
  SDA_LOW();
  DelayMicroSeconds(10);  // hold time start condition (t_HD;STA)
  SCL_LOW();
  DelayMicroSeconds(10);
}

//==============================================================================
void I2c_StopCondition(void){
//==============================================================================
  SCL_LOW();
  DelayMicroSeconds(1);
  SDA_LOW();
  DelayMicroSeconds(1);
  SCL_OPEN();
  DelayMicroSeconds(10);  // set-up time stop condition (t_SU;STO)
  SDA_OPEN();
  DelayMicroSeconds(10);
}

//==============================================================================
etError I2c_WriteByte(u8t txByte){
//==============================================================================
  u8t     mask;
  etError error = NO_ERROR;
  for(mask = 0x80; mask > 0; mask >>= 1)// shift bit for masking (8 times)
  {
    if((mask & txByte) == 0) SDA_LOW(); // masking txByte, write bit to SDA-Line
    else                     SDA_OPEN();
    DelayMicroSeconds(1);               // data set-up time (t_SU;DAT)
    SCL_OPEN();                         // generate clock pulse on SCL
    DelayMicroSeconds(5);               // SCL high time (t_HIGH)
    SCL_LOW();
    DelayMicroSeconds(1);               // data hold time(t_HD;DAT)
  }
  SDA_OPEN();                           // release SDA-line
  SCL_OPEN();                           // clk #9 for ack
  DelayMicroSeconds(1);                 // data set-up time (t_SU;DAT)
  if(SDA_READ()) error = ACK_ERROR;       // check ack from i2c slave
  SCL_LOW();
  DelayMicroSeconds(20);                // wait to see byte package on scope
  return error;                         // return error code
}

//==============================================================================
u8t I2c_ReadByte(etI2cAck ack){
//==============================================================================
  u8t mask;
  u8t rxByte = NO_ERROR;
  SDA_OPEN();                            // release SDA-line
  for(mask = 0x80; mask > 0; mask >>= 1) // shift bit for masking (8 times)
  { 
    SCL_OPEN();                          // start clock on SCL-line
    DelayMicroSeconds(1);                // data set-up time (t_SU;DAT)
    while(SCL_READ == 0);
    DelayMicroSeconds(3);                // SCL high time (t_HIGH)
    if(SDA_READ()) rxByte = rxByte | mask; // read bit
    SCL_LOW();
    DelayMicroSeconds(1);                // data hold time(t_HD;DAT)
  }
  if(ack == ACK) SDA_LOW();              // send acknowledge if necessary
  else           SDA_OPEN();
  DelayMicroSeconds(1);                  // data set-up time (t_SU;DAT)
  SCL_OPEN();                            // clk #9 for ack
  DelayMicroSeconds(5);                  // SCL high time (t_HIGH)
  SCL_LOW();
  SDA_OPEN();                            // release SDA-line
  DelayMicroSeconds(20);                 // wait to see byte package on scope
  return rxByte;                         // return error code
}
