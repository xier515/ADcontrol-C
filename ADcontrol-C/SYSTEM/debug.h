#ifndef _DEBUG_H
#define _DEBUG_H

#include "Rs485.h"
#include "DME3000.h"
#include "pre_send.h"


extern char goback;
void Receive_Debug(char data);
void debugtask(void);

#endif

