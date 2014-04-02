#ifndef MOTOR_HH
#define MOTOR_HH

void                motor_init(void);
int                 motor_calcSpeed(int8_t error);
void                motor_setSpeed(int rpm);

void                motor_writePWM(int pwm);
int                 motor_getRPM(void);

int                 tachometer_value2rpm(uint16_t, uint8_t);
uint16_t            tachometer_read(void);



/* not in use */
//int                 motorPI(int);

#endif
