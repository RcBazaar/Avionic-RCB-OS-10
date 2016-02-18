#ifndef _PPM_DECODE_H_
#define _PPM_DECODE_H_

#include "stm32f10x.h"

#define PPMINNUM	10
#define PPMIN_HIGH	4000
#define PPMIN_NEU	1500
#define PPMIN_LOW	400

void PpmInCfg(void);
void PpmInCtl(u8 onoff);

extern s16 PpmInValue[PPMINNUM];
extern s16 Std_PpmInValue[PPMINNUM];


#endif
