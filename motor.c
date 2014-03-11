#include "motor.h"


void calcMotorSpeed(char error){
	
}

void setMotorSpeed(int speed){
	
}
uint8_t motorPI(uint8_t error){
	
}
inline uint16_t readTachometer(void){
	uint16_t temp = TCNT5;
	//TODO ???
	TCNT5H = 0;
	TCNT5L = 0;
	return temp;
}
