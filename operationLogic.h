/*
* Operation logic function declarations
*
*
*/

#ifndef OPERATION_LOGIC_HH
#define OPERATION_LOGIC_HH
void calcControl(uint8_t error, char speed, uint8_t angle);
char calcMotorSpeed(char);
char calcError(uint8_t sensor_values);

void executeControl(char speed, uint8_t angle);
int  PID(char);
char pidValue2Deg(char val);
uint8_t readBumper(void);

void setNewState(uint8_t newState);
void setState(char error);
#endif
