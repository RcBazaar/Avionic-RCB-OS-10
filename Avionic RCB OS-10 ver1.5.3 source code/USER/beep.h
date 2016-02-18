#ifndef _BEEP_H_
#define _BEEP_H_

#include "stm32f10x.h"

//“Ù¿÷¡–±Ì
extern const u8 MusicStartup[];
extern const u8 MusicTrimZero[];
extern const u8 MusicTrimMax[];
extern const u8 MusicEnter[];
extern const u8 MusicBatLow[];
extern const u8 MusicTimeout[];

void Beep_Cofiguration(void);
void Beep_Set_Vibration(u8 state);
void Beep(u16 frequency, u8 volume);
void BeepMusic(const u8 *music);
void BeepShort(u16 frequency,u8 volume,u8 tm);
void BeepHandler(void);

#endif
