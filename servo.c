#include "includes.h"

void writeServoControl(char deg)
{
    if(deg < SERVO_VALUE_MIN)
    {
        deg = SERVO_VALUE_MIN;
        reportError(ERR_SERVO_UNDER_MIN_CONTROL);
    }
    else if(deg > CONTROL_VALUE_MAX)
    {
        deg = CONTROL_VALUE_MAX;
        reportError(ERR_SERVO_OVER_MAX_CONTROL);
    }

    TCNT1 = 0;
    OCR1A = SERVO_PULSE_MIN + (deg + 90) * SERVO_PULSE_WIDTH;
}

