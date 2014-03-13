#ifndef MOTOR_HH
#define MOTOR_HH

void setMotorSpeed(int);
int tachometer2speed(uint16_t, uint8_t);
int motorPI(int);
uint16_t readTachometer(void);

#endif