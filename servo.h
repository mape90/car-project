#ifndef SERVO_HH
#define SERVO_HH

#include "includes.h"

void                servo_init(void);
int8_t              servo_getAngle(void);
void                servo_writeControl(int8_t);

#endif
