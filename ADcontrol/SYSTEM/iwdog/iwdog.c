#include "iwdog.h"

void My_IWDGInit()  //2s
{
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(IWDG_Prescaler_64);
    IWDG_SetReload(1000);
    IWDG_Enable();
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
    IWDG_ReloadCounter();
    
    
}

void IWDG_Feed()
{
    IWDG_ReloadCounter();
}

