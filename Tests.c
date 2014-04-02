/**-------------------------------------*\
* Test Functions for testing operations  *
\*--------------------------------------*/

#include "includes.h"

extern char gPrevPositionError;
extern volatile bool gFindRoadTimerElapsed;
extern bool gStoppedTextWritten;

void test_motor_loop()
{ //dummy test
    //setMotorSpeed(300);
	motor_writePWM(5000);
}

void test_tachometer_loop(){
    motor_writePWM(5000);
    LCD_clear();
    LCD_writeInt((int)tachometer_read(),9);
}

void test_tachometer_rpm_loop(){
    motor_writePWM(5000);
    LCD_clear();
    LCD_writeInt(tachometer_value2rpm(tachometer_read(),1),10);
}

/* motorPI is disabled (replaced with atmel pid)
void test_tachometer_PI_loop(){
    uint16_t rpm = tachometer2rpm(readTachometer(),1);
    int controll_val = motorPI(1000-rpm);
    writeMotorPWM(controll_val);
    //writeMotorPWM(5000);
    //LCD_clear();
    //LCD_Write_int((int)rpm, 10);
}
*/

void test_controll_loop()
{ //test everything elese than states
    if(!gFindRoadTimerElapsed){
        int angle, speed;
        int8_t error = control_calcPositionError(bumper_read());
        control_calcParameters(error, &speed, &angle);
        control_execute(speed, angle);
        gPrevPositionError = error;
    }else{
        motor_writePWM(0);
        if(!gStoppedTextWritten){
            gStoppedTextWritten = true;
            LCD_clear();
            LCD_writeString("      Stopped",8);
        }
    }
}

void test_servo_loop()
{ //dummy test
    for(int i = 0;i<=45;i=i+1){
        servo_writeControl(i);
        _delay_ms(3);
    }
    for(int i = 45;i>=-45;i=i-1){
        servo_writeControl(i);
        _delay_ms(3);
    }
    for(int i = -45;i<=0;i=i+1){
        servo_writeControl(i);
        _delay_ms(3);
    }
}
