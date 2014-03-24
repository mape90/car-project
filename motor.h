#ifndef MOTOR_HH
#define MOTOR_HH

#include "includes.h"

void setMotorSpeed(int);
int calcMotorSpeed(int);
int tachometer2speed(uint16_t, uint8_t);
int motorPI(int);
uint16_t readTachometer(void);

#endif