#include "servo.h"

void setServoStatus(uint8_t status){
    if(status){
        TCCR1A |= (1<<COM1A1);//enable servo
    }else{
        TCCR1A &= ~(1<<COM1A1);//dissable servo
    }
}

void writeServoControl(char deg){
    if(deg < min_servo_value){
        deg = min_servo_value;
        reportError(SEVO_UNDER_MIN_CONTROLL);
    }else if(deg > max_control_value){
        deg = max_control_value;
        reportError(SERVO_OVER_MAX_CONTROL);
    }
    TCNT1 = 0;
    OCR1A = SERVO_MIN_PULSE + (deg + 90) * PULSE_WIDTH; 
}
 
