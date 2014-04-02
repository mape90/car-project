/*
* Operation logic function declarations
*/

#ifndef OPERATION_LOGIC_HH
#define OPERATION_LOGIC_HH

void            control_calc(int8_t error, int* speed, int* angle);
void            control_execute(int speed, int angle);


uint8_t         bumper_read(void);
void            bumper_calcValue(void);

void            setFindTimer(void);
int8_t          calcPositionError(uint8_t sensor_values);

/* functions not in use */
//int  PID(int8_t);                           // our own implementation of servo control PID that was replaced with Atmel pid lib
//void setNewState(uint8_t newState);
//void setState(int8_t error);
//void runCar(void);
//void stopCar(void);
#endif
