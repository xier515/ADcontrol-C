#ifndef _logic_h
#define _logic_h

#include <stm32f4xx.h>
extern unsigned int RunTime;
u8 CalculateAverageTemp(void);
u8 CalculateAverageHumi(void);
void HumidityCheck(void);
void TempCheck(void);
void CheckTask(void);
#endif

