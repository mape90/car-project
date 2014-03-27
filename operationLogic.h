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
void calcControl(int8_t error, int* speed, int* angle);
int calcMotorSpeed(int8_t);
int8_t calcError(uint8_t sensor_values);

void executeControl(int speed, int angle);
int  PID(int8_t);
uint8_t readBumper(void);

void setNewState(uint8_t newState);
void setState(char error);
void setFindTimer(void);
void calcBumperValue(void);
bool isValidBumperValue(uint8_t bumperVal);
#endif
