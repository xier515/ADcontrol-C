//==============================================================================
//    S E N S I R I O N   AG,  Laubisruetistr. 50, CH-8712 Staefa, Switzerland
//==============================================================================
// Project   :  SHTC1 Sample Code (V1.0)
// File      :  shtc1.h (V1.0)
// Author    :  RFU
// Date      :  26-Nov-2012
// Controller:  STM32F100RB
// IDE       :  µVision V4.60.0.0
// Compiler  :  Armcc
// Brief     :  Sensor Layer: Definitions of commands and functions for sensor
//                            access.
//==============================================================================

#ifndef SHTC1_H
#define SHTC1_H

//-- Includes ------------------------------------------------------------------
#include "system.h"

//-- Defines -------------------------------------------------------------------
// CRC
#define POLYNOMIAL  0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001

//-- Enumerations --------------------------------------------------------------
// Sensor Commands
typedef enum{
  READ_ID            = 0xEFC8, // command: read ID register
  SOFT_RESET         = 0x805D, // soft reset
  MEAS_T_RH_POLLING  = 0x7866, // meas. read T first, clock stretching disabled
  MEAS_T_RH_CLOCKSTR = 0x7CA2, // meas. read T first, clock stretching enabled
  MEAS_RH_T_POLLING  = 0x58E0, // meas. read RH first, clock stretching disabled
  MEAS_RH_T_CLOCKSTR = 0x5C24  // meas. read RH first, clock stretching enabled
}etCommands;

// I2C address
typedef enum{
  I2C_ADR_W        = 0xE0,   // sensor I2C address + write bit
  I2C_ADR_R        = 0xE1    // sensor I2C address + read bit
}etI2cHeader;

//==============================================================================
void SHTC1_Init(void);
//==============================================================================
// Initializes the I2C bus for communication with the sensor.
//------------------------------------------------------------------------------

//==============================================================================
etError SHTC1_GetId(u16t *id);
//==============================================================================
// Gets the ID from the sensor.
//------------------------------------------------------------------------------
// input:  *id          pointer to a integer, where the id will be stored
//
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      CHECKSUM_ERROR = checksum mismatch
//                      NO_ERROR       = no error

//==============================================================================
etError SHTC1_GetTempAndHumi(signed int *temp, signed int *humi);
//==============================================================================
// Gets the temperature [°C] and the humidity [%RH].
//------------------------------------------------------------------------------
// input:  *temp        pointer to a floating point value, where the calculated
//                      temperature will be stored
//         *humi        pointer to a floating point value, where the calculated
//                      humidity will be stored
// 
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      CHECKSUM_ERROR = checksum mismatch
//                      NO_ERROR       = no error
//
// remark: If you use this function, then the sensor blocks the I2C-bus with
//         clock stretching during the measurement.

//==============================================================================
etError SHTC1_GetTempAndHumiPolling(signed int *temp, signed int *humi);
//==============================================================================
// Gets the temperature [°C] and the humidity [%RH]. This function polls every
// 1ms until measurement is ready.
//------------------------------------------------------------------------------
// input:  *temp        pointer to a floating point value, where the calculated
//                      temperature will be stored
//         *humi        pointer to a floating point value, where the calculated
//                      humidity will be stored
// 
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      CHECKSUM_ERROR = checksum mismatch
//                      NO_ERROR       = no error

//==============================================================================
etError SHTC1_SoftReset(void);
//==============================================================================
// Calls the soft reset mechanism that forces the sensor into a well-defined
// state without removing the power supply.
//------------------------------------------------------------------------------
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      NO_ERROR       = no error

//==============================================================================
etError SHTC1_StartWriteAccess(void);
//==============================================================================
// Writes a start condition and the sensor I2C address with the write flag.
//------------------------------------------------------------------------------
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      NO_ERROR       = no error

//==============================================================================
etError SHTC1_StartReadAccess(void);
//==============================================================================
// Writes a start condition and the sensor I2C address with the read flag.
//------------------------------------------------------------------------------
// return: error:       ACK_ERROR      = no acknowledgment from sensor
//                      NO_ERROR       = no error

//==============================================================================
void SHTC1_StopAccess(void);
//==============================================================================
// Writes a stop condition.
//------------------------------------------------------------------------------

//==============================================================================
etError SHTC1_Read2BytesAndCrc(u16t *data);
//==============================================================================
// Reads two bytes plus the checksum and verifies this. If the checksum
// verification is successful, then the two bytes stored in a 16-bit integer.
//------------------------------------------------------------------------------
// input:  *data        pointer to a 16-bit int, where the data will be stored
//
// return: error:       CHECKSUM_ERROR = checksum does not match
//                      NO_ERROR       = checksum matches

//==============================================================================
etError SHTC1_WriteCommand(etCommands cmd);
//==============================================================================
// Writes command to the sensor.
//------------------------------------------------------------------------------
// input:  cmd          command which is to be written to the sensor
//
// return: error:       ACK_ERROR = no acknowledgment from sensor
//                      NO_ERROR  = no error

//==============================================================================
etError SHTC1_CheckCrc(u8t data[], u8t nbrOfBytes, u8t checksum);
//==============================================================================
// Calculates checksum for n bytes of data and compares it with expected
// checksum.
//------------------------------------------------------------------------------
// input:  data[]       checksum is built based on this data
//         nbrOfBytes   checksum is built for n bytes of data
//         checksum     expected checksum
//
// return: error:       CHECKSUM_ERROR = checksum does not match
//                      NO_ERROR       = checksum matches

//==============================================================================
signed short SHTC1_CalcTemperature(u16t rawValue);
//==============================================================================
// Calculates the temperature [°C] as a floating point value from the raw data
// that are read from the sensor.
//------------------------------------------------------------------------------
// input:  rawValue     temperature raw value (16bit scaled)
//
// return:              temperature [°C] as a floating point value

//==============================================================================
signed short SHTC1_CalcHumidity(u16t rawValue);
//==============================================================================
// Calculates the relative humidity [%RH] as a floating point value from the raw
// data that are read from the sensor.
//------------------------------------------------------------------------------
// input:  rawValue     humidity raw value (16bit scaled)
//
// return:              relative humidity [%RH] as a floating point value

signed short GetSHTCData(void);

#endif
