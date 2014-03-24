#ifndef UI_hh
#define UI_hh

#include "includes.h"

void updateLcd(void);

void LCDWrite(void);

void LCDClearErrors(void);

void LCDWriteError(char*);

inline void setLcdTimer(void);


#endif