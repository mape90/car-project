#ifndef SERVO_HH
#define SERVO_HH

#include "includes.h"

void servo_init(void);
char getServoAngle(void);
void writeServoControl(int);

#endif
