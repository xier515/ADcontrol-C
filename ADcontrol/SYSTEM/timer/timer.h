#ifndef _timer_h
#define _timer_h
#include <stm32f4xx.h>

extern u8 _2sflag;
extern u8 _1msflag;
extern u8 _5sflag;
extern u8 _1sflag;
extern int tempUpdataCnt;
void TaskTimerInit(void);

#endif

