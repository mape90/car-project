#ifndef MOTOR_HH
#define MOTOR_HH

void setMotorSpeed(int speed);
int tachometer2speed(uint16_t val, uint8_t dir);
int motorPI(int errorInSpeed);
uint16_t readTachometer(void);
void writeMotorPWM(int pwm);

#endif
