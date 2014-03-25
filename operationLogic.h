/*
* Operation logic function declarations
*
*
*/

#ifndef OPERATION_LOGIC_HH
#define OPERATION_LOGIC_HH

void runCar(void);
void stopCar(void);
void findRoad(void);

void calcControl(uint8_t error, char* speed, uint8_t* angle);
char calcMotorSpeed(char);
char calcError(uint8_t sensor_values);

void executeControl(int speed, uint8_t angle);
int  PID(char);
uint8_t readBumper(void);

void setNewState(uint8_t newState);
void setState(char error);
void setFindTimer(void);
#endif
