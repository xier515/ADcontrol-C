//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  SHTC1 Sample Code (V1.0)
// File      :  i2c_hal.h (V1.0)
// Author    :  RFU
// Date      :  26-Nov-2012
// Controller:  STM32F100RB
// IDE       :  µVision V4.60.0.0
// Compiler  :  Armcc
// Brief     :  I2C hardware abstraction layer 
//==============================================================================

#ifndef I2C_HAL_H
#define I2C_HAL_H

//-- Includes ------------------------------------------------------------------
#include "system.h"

// //-- Defines -------------------------------------------------------------------
// // I2C IO-Pins                         /* -- adapt the defines for your uC -- */
// // SDA on port B, bit 9
// #define SDA_LOW()  GPIO_ResetBits(GPIOB,GPIO_Pin_7) // set SDA to low
// #define SDA_OPEN() GPIO_SetBits(GPIOB,GPIO_Pin_7) // set SDA to open-drain
// #define SDA_READ   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)     // read SDA

// // SCL on port B, bit 8                /* -- adapt the defines for your uC -- */
// #define SCL_LOW()  GPIO_ResetBits(GPIOB,GPIO_Pin_6) // set SCL to low
// #define SCL_OPEN() GPIO_SetBits(GPIOB,GPIO_Pin_6) // set SCL to open-drain
// #define SCL_READ   GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_6)     // read SCL

// // I2C IO-Pins                         /* -- adapt the defines for your uC -- */
// // SDA on port B, bit 9
// #define SDA2_LOW()  GPIO_ResetBits(GPIOC,GPIO_Pin_9) // set SDA to low
// #define SDA2_OPEN() GPIO_SetBits(GPIOC,GPIO_Pin_9) // set SDA to open-drain
// #define SDA2_READ   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)     // read SDA

// // SCL on port B, bit 8                /* -- adapt the defines for your uC -- */
// #define SCL2_LOW()  GPIO_ResetBits(GPIOA,GPIO_Pin_8) // set SCL to low
// #define SCL2_OPEN() GPIO_SetBits(GPIOA,GPIO_Pin_8) // set SCL to open-drain
// #define SCL2_READ   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)     // read SCL

//-- Enumerations --------------------------------------------------------------

extern unsigned char I2C_Num;
// I2C acknowledge
typedef enum{
  ACK    = 0,
  NO_ACK = 1,
}etI2cAck;

void DelayMicroSeconds(unsigned int nms);

//==============================================================================
void I2c_Init(void);
//==============================================================================
// Initializes the ports for I2C interface.
//------------------------------------------------------------------------------

//==============================================================================
void I2c_StartCondition(void);
//==============================================================================
// Writes a start condition on I2C-Bus.
//------------------------------------------------------------------------------
// remark: Timing (delay) may have to be changed for different microcontroller.
//       _____
// SDA:       |_____
//       _______
// SCL:         |___

//==============================================================================
void I2c_StopCondition(void);
//==============================================================================
// Writes a stop condition on I2C-Bus.
//------------------------------------------------------------------------------
// remark: Timing (delay) may have to be changed for different microcontroller.
//              _____
// SDA:   _____|
//            _______
// SCL:   ___|

//==============================================================================
etError I2c_WriteByte(u8t txByte);
//==============================================================================
// Writes a byte to I2C-Bus and checks acknowledge.
//------------------------------------------------------------------------------
// input:  txByte       transmit byte
//
// return: error:       ACK_ERROR = no acknowledgment from sensor
//                      NO_ERROR  = no error
//
// remark: Timing (delay) may have to be changed for different microcontroller.

//==============================================================================
u8t I2c_ReadByte(etI2cAck ack);
//==============================================================================
// Reads a byte on I2C-Bus.
//------------------------------------------------------------------------------
// input:  ack          Acknowledge: ACK or NO_ACK
//
// return: rxByte
//
// remark: Timing (delay) may have to be changed for different microcontroller.

#endif
