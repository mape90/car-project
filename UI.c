#include "UI.h"

void updateLcd(void){
    if(gui_can_update){
        LCDWrite();
        setLcdTimer();
    }else{
        //Do nothing
    }
}

void LCDWrite(void){
    //write getMotorRPM()
    //write getServoAngle()
    //i.e: speed: -100 rpm, servo angle -10 deg
    if(state == wait_state && has_error){
        LCDClearErrors();
    }
}

void LCDClearErrors(void){
    //clear all error texts
    has_error = false;
}

void LCDWriteError(char* msg){
    has_error = ture;
    //write errror
}

inline void setLcdTimer(void){
    timer_set_value[timer2] = lcd_wait_time;
}
