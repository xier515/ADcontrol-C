#ifndef _OLDXINGFENG_H
#define _OLDXINGFENG_H

#include "stm32f4xx.h"

void Xinfeng_Init(void);
void Xinfeng_Kaiguan(u8 en);
void Xinfeng_Tongfeng(u8 en);
void Xinfeng_Shuileng(u8 en);
void Xinfeng_Qingxi(u8 en);

void XinfengStateRead(void);
#endif

