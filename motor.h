#ifndef MOTOR_HH
#define MOTOR_HH



void calcMotorSpeed(char error);
void setMotorSpeed(int speed);
int tachometer2speed(uint16_t);
uint8_t motorPI(uint8_t error);
inline uint16_t readTachometer(void);

#endif