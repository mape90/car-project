#include "includes.h"

static char gWheelOrientation = 0;

void servo_init(void)
{
    DDRB |= (1 << SERVO_PIN);
    ICR1 = 40000;
    TCCR1A |= (1<<WGM11) | (1<<COM1A1);
    TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11);
}


char getServoAngle(void){
    return gWheelOrientation;
}

void writeServoControl(char deg){
    if(gWheelOrientation != deg){
        if(deg < SERVO_VALUE_MIN){
            deg = SERVO_VALUE_MIN;
            reportError(ERR_SERVO_UNDER_MIN_CONTROL);
        }else if(deg > SERVO_VALUE_MAX){
            deg = SERVO_VALUE_MAX;
            reportError(ERR_SERVO_OVER_MAX_CONTROL);
        }
        TCNT1 = 0;
        OCR1A = SERVO_PULSE_MIN + (deg + 90) * SERVO_PULSE_WIDTH;
        gWheelOrientation = deg;
    }
}

