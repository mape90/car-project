#ifndef SERVO_HH
#define SERVO_HH

#include "includes.h"

void servo_init(void);
int8_t getServoAngle(void);
void writeServoControl(int8_t);

#endif
