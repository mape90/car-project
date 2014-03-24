#include "motor.h"
#include "includes.h"

void setMotorSpeed(int rpm){
    tacho_val = readTachometer();
    current_rpm = tachometer2rpm(tacho_val, (rpm<0)? MOTOR_BACKWARD : MOTOR_FORWARD);
    writeMotorPWM(motorPI(rpm - current_rpm));
}

char getMotorRPM(void){
    return current_rpm;
}

int calcMotorSpeed(int error){
    if(error > 8 - && error < 8)
        return max_speed - abs(error)*speed_reduce;
    else
        return 0;
}

void writeMotorPWM(int pwm){
    if(last_pwm != pwm){
        //limit control values
        if(abs(pwm) > max_motor_controll){
            pwm = (pwm < 0) ? -max_motor_controll : max_motor_controll;
        }
        //limit acceleration
        if(abs(pwm - last_pwm) > max_motor_acc){
            pwm = last_pwm + (((pwm - last_pwm) < 0) ? -max_motor_acc : max_motor_acc);
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
        last_pwm = pwm;
    }
}
const uint8_t tic_per_rotation = 2;

//val tacho values from last 10 readings ~1sec
int tachometer2rpm(uint16_t val, uint8_t dir){
    val = val * 1000/loop_time_ms/tacho_buffer_size/tic_per_rotation*60;  // 100ms 2 tic per rotation. 1 tic -> 10 tic per sec 5 rotation per sec 300rpm
    if(dir == MOTOR_FORWARD){
        return int(val)
    }else{
        return -1*int(val);
    }
}

int motorPI(int error_in_speed){
	int control = P_motor * error_in_speed;
    motor_I_sum += I_motor * error_in_speed;
    
    //integer wind up
    if(abs(motor_I_sum) > max_motor_I_value){
        motor_I_sum = (motor_I_sum < 0) ? -max_motor_I_value : max_motor_I_value;
    }
    control += motor_I_sum;

    return control;
}

//this function gives sum of 10 last readings from tacho meter 
uint16_t readTachometer(void){
	buffer[tacho_buffer_cursor] = TCNT5;
    tacho_buffer_cursor = (tacho_buffer_cursor + 1)%10;
	TCNT5 = 0;
    uint16_t temp = 0;
    for(uint8_t i = 0; i< tacho_buffer_size; i++){
        temp += buffer[i];
    }
	return temp;
}
