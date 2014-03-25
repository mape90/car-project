#ifndef MOTOR_HH
#define MOTOR_HH

#include "includes.h"

void setMotorSpeed(int rpm);
char calcMotorSpeed(char error);
int tachometer2rpm(uint16_t, uint8_t);
int motorPI(int);
uint16_t readTachometer(void);
void writeMotorPWM(int pwm);
int getMotorRPM(void);

#endif
