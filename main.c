#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdbool.h>

#include "includes.h"
#include "motor.h"
#include "servo.h"
#include "UI.h"

//Function definations
/*
void readIOValues(void);
void filterIOData(void);
void calcControl(void);
void pidValue2Deg(char);
void calcMotorSpeed(char);
char calcError(void);
void executeControl(void);
void updateLcd(void);
int  PID(char);


void setServoStatus(uint8_t enable);
void writeServoControl(char deg);
*/


//--------------TIMER-----------------//

inline void dissable_wait(void){
    need_waiting = false;
}
inline void timer_2_function(void){

}
inline void timer_3_function(void){
    
}

ISR(TIMER0_OVF_vect){
    for(uint8_t i = 0;i < timer_max_count;i++){
        if(timer_set_value[i] > 0){
            if(timers_current_value[i] > timer_set_value[i]){
                timers_current_value[i] = 0; 
                timer_set_value[i] = 0;
                switch(i){
                    case timer_1:
                        dissable_wait();
                        break;
                    case timer_2:
                        timer_2_function();
                        break;
                    case timer_3:
                        timer_3_function();
                        break;
                }
            }else{
                timers_current_value[i]++;
            }
        }
    }
}

ISR(INT5_vect){
	handleButton();
}

//-----------------------------------//



void main(void){
    setup();
    while(1){
		loop();
		waitUntilTimerEnd();
    }
}

/*
 * This function will 
 * 
 */
void waitUntilTimerEnd(){
	while(need_waiting){}
	need_waiting = true;
	timer_set_value[timer_1] = loop_time_ms;
}

void setup(void){
        
    //init timer 0 (Events)
    TCCR0B |= ((1 << CS01)|(1 << CS00)); // 64
    TCNT0 = 0;
    TIMSK0 |= _BV(TOIE0);
  
    //init timer 1 (Servo)
    ICR1 = 40000;
    TCCR1A |= (1<<WGM11) | (1<<COM1A1);
    TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11);
    
    //init timer 4 (Motor)
    DDRH |= (1 << 3);  
    TCCR4A = (1 << COM4A1) | (1 << COM4B1) | (1 << WGM42) | (1 << WGM41);
    OCR4A = 0; //set motor speed to zero
    DDRK |= _BV(0) | _BV(1); //INA and INB
    PORTK = 0x00;
    
    
    //init timer 5 (Tachometer)
	TCCR5B = _BV(ICNC5)| _BV(CS52) | _BV(CS51); //ICNC5 enables filtering
    TCNT5 = 0;
    
	timer_set_value[timer_1] = loop_time_ms;
    
    writeServoControl(0);
    sei();
}

void loop(void){
	if(state == running_state || state == finding_road_state){
		char error = calcError(readBumper());
		setState(error);
		calcControl(error);
		executeControl();
		last_error = error;
	}else{
		//wait_state, road_not_found
		//do nothing
	}
    updateLcd();
}

void setNewState(uint8_t new_state){
	uint8_t temp = SREG;
	cli();
	previous_state = state;
	state = new_state;
	SREG = temp;
}

void handleFindTimer(){
	setNewState(road_not_found);
}

void setFindTimer(){
	//set timer on if it isnt running
}

void handleButton(void){
	switch(state){
		case wait_state:
			setNewState(running_state);
			break;
		case running_state:
		case road_not_found:
			setNewState(wait_state);
			break;
		default:
			//LCD info?
			break;
	}
}

void setState(char error){
	if(error != last_error && error == goal_point){
		lap_count++;
		if(lap_count >= max_lap_count){
			setNewState(wait_state);
		}
	}else if(error == no_reference_point){
		setNewState(finding_road_state);
	}else{
        if(state != running_state){
            setNewState(running_state);
        }
	}
}

inline void readBumper(void){
    return SENSORS_PIN; //read sensor port data
}

void calcControl(void){
	char error = calcError();
	if(error == no_reference_point){
		//do what is needed to do when track is lost
	}else if(erro == goal_point){
        direction = pidValue2Deg(PID(0));
        motor_speed = calcMotorSpeed(0);
    }else{
		direction = pidValue2Deg(PID(error));
		motor_speed = calcMotorSpeed(error);
	}
}

void pidValue2Deg(char val){
	return val;
}

char calcError(uint8_t sensor_values){
	char error = 0;
	char most_left = -1;
	char most_right = -1;
	for(uint8_t i = 0;i < 8;i++){
		if(sensor_values & (0x80 >> i)){
			most_left = i;
			break;
		}
	}
	for(uint8_t i = 0;i < 8;i++){
		if(sensor_values & (0x01 << i)){
			most_right = i;
			break;
		}
	}
	if(most_left < 0 || most_right < 0){//no reference point found out of track
		error = no_reference_point;
	}else if(abs((7-most_left)-most_right) > goal_min_wide){
        for(uint8_t i = 1;i<goal_min_wide;i++){
            if(!(sensor_values & (0x01 << most_right+i))){
                error = 1;
                break;
            }
        }
        if(error){
            error = goal_point;
        }else{
            error = 0;
        }
    }
    if(error == 0){
        if((7-most_left) == most_right){ //same only one point
            error = 7-2*most_right;
        }else if(most_left < most_right){ //use left as controll
            if(sensor_values & (0x80 >> (most_left+1)) ){ // chek if line is between sensors
                error = -6 + 2*most_left;
            }else{
                error = -7 + 2*most_left;
            }
            
        }else{ //use right as controll
            if(sensor_values & (0x01 << (most_right+1)) ){ // chek if line is between sensors
                error = 6 - 2 * most_right;
            }else{
                error = 6 - 2 * most_right;
            }
        }
    }
	return error;
}

void executeControl(void){
    //Servo
    //motor
}

int PID(char error){
    int control = P * error;
    integer_sum += I*error;
    
    if(abs(integer_sum) > integer_max){//integer wind up
        integer_sum = (integer_sum < 0) ? -integer_max : integer_max;
    }
    
    control += D*(error-last_error) + integer_sum;
    
    if(abs(control) > max_control_value){//limit control values
        control = (control < 0) ? -max_control_value : max_control_value;
    }
    return control;
}
