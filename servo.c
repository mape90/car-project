#include "servo.h"

char getServoAngle(void){
    return wheel_orientation;
}

void writeServoControl(char deg){
    if(wheel_orientation != deg){
        if(deg < min_servo_value){
            deg = min_servo_value;
            LCDWriteError(SEVO_UNDER_MIN_CONTROLL);
        }else if(deg > max_control_value){
            deg = max_control_value;
            LCDWriteError(SERVO_OVER_MAX_CONTROL);
        }
        TCNT1 = 0;
        OCR1A = SERVO_MIN_PULSE + (deg + 90) * PULSE_WIDTH; 
        wheel_orientation = deg;
    }
}
 
