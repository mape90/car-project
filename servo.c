#include "includes.h"

static int8_t gWheelOrientation = 0;

void servo_init(void)
{
    DDRB |= (1 << SERVO_PIN);
    ICR1 = 40000;
    TCCR1A |= (1<<WGM11) | (1<<COM1A1);
    TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11);
}


int8_t servo_getAngle(void){
    return gWheelOrientation;
}

void servo_writeControl(int8_t deg){
    if(gWheelOrientation != deg){
        if(deg < SERVO_VALUE_MIN){
            deg = SERVO_VALUE_MIN;
            //reportError(ERR_SERVO_UNDER_MIN_CONTROL);
        }else if(deg > SERVO_VALUE_MAX){
            deg = SERVO_VALUE_MAX;
            //reportError(ERR_SERVO_OVER_MAX_CONTROL);
        }
        OCR1A = SERVO_PULSE_MIN + (deg + 90) * SERVO_PULSE_WIDTH;
        gWheelOrientation = deg;
    }
}

