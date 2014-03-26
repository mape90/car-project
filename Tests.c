#include "includes.h"

extern char gLastError;

void test_motor_loop()
{ //dummy test
    //setMotorSpeed(300);
	writeMotorPWM(5000);
}

void test_controll_loop()
{ //test everything elese than states
    uint8_t angle;
    char speed;

    char error = calcError(readBumper());

    //setState(error);
    calcControl(error, &speed, &angle);
    LCD_clear();
    LCD_error(error);

	//LCD_Write_String(error,1);

	//_delay_ms(500);
    executeControl(0, angle);
    gLastError = error;
}

void test_servo_loop()
{ //dummy test
    for(int i = 0;i<=45;i=i+1){
        writeServoControl(i);
        _delay_ms(3);
    }
    for(int i = 45;i>=-45;i=i-1){
        writeServoControl(i);
        _delay_ms(3);
    }
    for(int i = -45;i<=0;i=i+1){
        writeServoControl(i);
        _delay_ms(3);
    }
}
