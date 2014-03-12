#include "motor.h"
#include "includes.h"

/*void calcMotorSpeed(char error){
	if(error == 0){
		return motor_straight_speed;
	}else if(error == -7 || error == 7){
		return motor_max_turn_speed;
	}else{
		return motor_mid_turn_speed;
	}
}*/

void setMotorSpeed(int speed){
    writeMotorPWM(motorPI(speed - tachometer2speed(readTachometer())));
}

void writeMotorPWM(int pwm){
    
}

int tachometer2speed(uint16_t val){
    //TODO implement
    return val;
}


int motorPI(int error_in_speed){
	int control = P_motor * error_in_speed;
    motor_I_sum += I_motor*error_in_speed;
    
    //integer wind up
    if(abs(motor_I_sum) > max_motor_I_value){
        motor_I_sum = (motor_I_sum < 0) ? -max_motor_I_value : max_motor_I_value;
    }
    control += motor_I_sum;
    
    //limit control values
    if(abs(control) > max_motor_controll){
        control = (control < 0) ? -max_motor_controll : max_motor_controll;
    }
    return control;
}

//one tick is 180 deg
uint16_t readTachometer(void){
	uint16_t temp = TCNT5;
	TCNT5 = 0;
	return temp;
}
