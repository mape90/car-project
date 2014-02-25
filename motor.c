#include "includes.h"

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 0

volatile uint16_t tacho_val = 0;

void setMotorSpeed(int speed){
    tacho_val = readTachometer();
    writeMotorPWM(motorPI(speed - tachometer2speed(tacho_val, (speed<0)? MOTOR_BACKWARD : MOTOR_FORWARD)));
}

void writeMotorPWM(int pwm) {
    //limit control values
    if(abs(pwm) > MOTOR_CONTROL_MAX){
        pwm = (pwm < 0) ? -1* MOTOR_CONTROL_MAX : MOTOR_CONTROL_MAX;
    }
    if(pwm == 0){
        PORTK = 0x00;       //break
        OCR4A = 0;          //set pwm sycle to zero
    }else if(pwm > 0){
        PORTK = 0x02;       //Clockwise
        OCR4A = pwm;
    }else{
        PORTK = 0x01;       //Counter-clockwise (CCW)
        OCR4A = abs(pwm);   //set pwm walue positive
    }
}

//TODO make calculation to transform tachoval to speed
int tachometer2speed(uint16_t val, uint8_t dir){
    if(dir == MOTOR_FORWARD){
        return (int)val;
    }else{
        return -1*(int)val;
    }
}

int motorPI(int error_in_speed){
	int control = P_motor * error_in_speed;
    int motor_I_sum = I_motor*error_in_speed;

    //integer wind up
    if(abs(motor_I_sum) > max_motor_I_value){
        motor_I_sum = (motor_I_sum < 0) ? -max_motor_I_value : max_motor_I_value;
    }
    control += motor_I_sum;

    return control;
}

//one tick is 180 deg ??
uint16_t readTachometer(void){
	uint16_t temp = TCNT5;
	TCNT5 = 0;
	return temp;
}
