#include "includes.h"

extern char gLastError;
extern volatile bool gFindRoadTimerElapsed;
void test_motor_loop()
{ //dummy test
    //setMotorSpeed(300);
	writeMotorPWM(5000);
}

void test_tachometer_loop(){
    writeMotorPWM(5000);
    LCD_clear();
    LCD_Write_int((int)readTachometer(),9);
}

void test_tachometer_rpm_loop(){
    writeMotorPWM(5000);
    LCD_clear();
    LCD_Write_int(tachometer2rpm(readTachometer(),1),10);
}

void test_tachometer_PI_loop(){
    uint16_t rpm = tachometer2rpm(readTachometer(),1);
    int controll_val = motorPI(1000-rpm);
    writeMotorPWM(controll_val);
    //writeMotorPWM(5000);
    //LCD_clear();
    //LCD_Write_int((int)rpm, 10);
}

bool writenText = false;

void test_controll_loop()
{ //test everything elese than states
    if(!gFindRoadTimerElapsed){
        int angle, speed;
        int8_t error = calcError(readBumper());
        calcControl(error, &speed, &angle);
        executeControl(speed, angle);
        gLastError = error;
    }else{
        writeMotorPWM(0);
        if(!writenText){
            writenText = true;
            LCD_clear();
            LCD_Write_String("      Stopped",8);
        }
    }
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
