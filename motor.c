#include "motor.h"



void calcMotorSpeed(char error){
	if(error == 0){
		return motor_straight_speed;
	}else if(error == -7 || error == 7){
		return motor_max_turn_speed;
	}else{
		return motor_mid_turn_speed;
	}
}

void setMotorSpeed(int speed){
	
}

uint8_t motorPI(uint8_t error){
	int control = P_motor * error;
    motor_I_sum += I_motor*error;
    
    if(abs(motor_I_sum) > integer_max){//integer wind up
        motor_I_sum = (motor_I_sum < 0) ? -integer_max : integer_max;
    }
    
    control += + motor_I_sum;
    
    if(abs(control) > max_control_value){//limit control values
        control = (control < 0) ? -max_control_value : max_control_value;
    }
    return control;
}
uint16_t readTachometer(void){
	uint16_t temp = TCNT5;
	TCNT5 = 0;
	return temp;
}
