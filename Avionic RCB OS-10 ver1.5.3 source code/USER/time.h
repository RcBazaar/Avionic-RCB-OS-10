#ifndef _TIME_H_
#define _TIME_H_

#include "stm32f10x.h"

void Time_Init(void);
uint32_t Time_NowUs(void);
uint32_t Time_NowMs(void);
void DelayUs(uint32_t nus);
void DelayMs(uint32_t nms);

#endif

