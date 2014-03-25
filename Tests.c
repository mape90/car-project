#include "includes.h"

extern char gLastError;

void test_motor_loop()
{ //dummy test
    setMotorSpeed(300);
}

void test_controll_loop()
{ //test everything elese than states
    uint8_t angle;
    char speed;
    char error = calcError(readBumper());
    //setState(error);
    calcControl(error, &speed, &angle);
    executeControl(0, angle);
    gLastError = error;
}

void test_servo_loop()
{ //dummy test
    for(int i = 0;i<=90;i++){
        writeServoControl(i);
        _delay_ms(10);
    }
    for(int i = 90;i>=-90;i--){
        writeServoControl(i);
        _delay_ms(10);
    }
    for(int i = -90;i<=0;i++){
        writeServoControl(i);
        _delay_ms(10);
    }
}
