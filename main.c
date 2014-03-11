#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

/*
 * he servo-PWM is connected to the A output of the Timer/Counter1 subsystem (pin PB5 of ATMega2560). 
 * The motor-PWM is connected to the A output of the Timer/Counter4 subsystem (pin PH3 of ATMega2560). 
 * 
 */

/*
 * Motor (Timer4)
 * INA - PK0 (ADC8)
 * INB - PK1 (ADC9)
 * ENA - PK2 (INPUT)
 * ENB - PK3 (INPUT)
 * 
 */

/*
 * Servo (Timer1)
 * PWM - PB5 (OC1A)
 * 
 */

/*
 * Tachometer (Timer5)
 * Signal - PL2 (T5)
 * 
 */

/*
 * LCD
 * TX - PD2 (RXD1)
 * TX - PD3 (TXD1)
 */

/*
 * Led
 * PC0 - led0 
 * PC1 - led1
 * 
 * Button (HIGH when not pushed)
 * PE5 (INT5)
 */


#define LEDS_PORT PORTC
#define LEDS_DDR  DDRC
#define LEDS_PIN  PINC

#define BUTTONS_PORT PORTA
#define BUTTONS_DDR  DDRA
#define BUTTONS_PIN  PINA

#define MOTOR_ON 1
#define MOTOR_OFF 0

#define SERVO_ON 1
#define SERVO_OFF 0

#define SEVO_UNDER_MIN_CONTROLL 10
#define SEVO_OVER_MAX_CONTROLL 11

#define PULSE_WIDTH 11

//CONSTANT VALUES
const char max_servo_value = 90;
const char min_servo_value = -90;

const uint16_t servo_min_pulse 2000

const uint8_t servo_pin = 3;
const uint8_t motor_pin_1 = ;
const uint8_t motor_pin_2 = ;

const uint8_t integer_max = 200;
const uint8_t max_control_value = 255;

const uint8_t P = 1;
const uint8_t I = 0;
const uint8_t D = 0;

const uint8_t no_reference_point = 200; 
const uint8_t goal_point = 100;

const uint8_t motor_straight_speed = 180;
const uint8_t motor_mid_turn_speed = 150;
const uint8_t motor_max_turn_speed = 100;


const uint8_t wait_state = 1;
const uint8_t running_state = 2;
const uint8_t finding_road_state = 3;
const uint8_t road_not_found = 4;


//GLOBAL VARIABLES
int last_error = 0;
int integer_sum = 0;

int servo_value = 0;
uint8_t motor_speed = 0;

uint8_t sensor_values = 0;


//Function definations
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

#define MOTOR_BREAK 0
#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 2

/*
 * 
 * 
 */

void writeMotorControl(uint8_t speed, uint8_t dir);
void setMotorPWM(uint8_t val);
void setMotorStatus(uint8_t enable);

uint8_t motorPI(uint8_t error);


//--------------TIMER-----------------//

const uint8_t timer_max_count = 3;
const uint8_t timer_1 = 1;
const uint8_t timer_2 = 2;
const uint8_t timer_3 = 3;

inline void timer_1_function(void){
    
}
inline void timer_2_function(void){

}
inline void timer_3_function(void){
    
}

uint16_t timers_current_value[timer_count];
uint16_t timer_set_value[timer_count];
;
ISR(TIMER0_OVF_vect){
    for(uint8_t i = 0;i < timer_max_count;i++){
        if(timer_set_value[i] > 0){
            if(timers_current_value[i] > timer_set_value[i]){
                timers_current_value[i] = 0; 
                timer_set_value[i] = 0;
                switch(i){
                    case timer_1:
                        timer_1_function();
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

//-----------------------------------//

void main(void){
    setup();
    while(1){
        loop();
    }
}

void setup(void){
    // init
	// wait for putton pressed to start driving
}

void loop(void){
    readIOValues();
    filterIOData();
    calcControl();
    executeControl();
    updateLcd();
}

void readIOValues(void){
    sensor_values = SENSORS_PIN; //read sensor port data
    tacho_val = tacho_count;
}

void filterIOData(void){
    return;
}

void calcControl(void){
	char error = calcError();
	if(error == no_reference_point){//do what is needed to do when track is lost
		
		direction = pidValue2Deg(PID(-7));
		motor_speed = calcMotorSpeed(-7);
	}else{
		direction = pidValue2Deg(PID(error));
		motor_speed = calcMotorSpeed(error);
	}
}

void pidValue2Deg(char val){
	return 
}

void calcMotorSpeed(char val){
	if(val == 0){
		return motor_straight_speed;
	}else if(val == -7 || val == 7){
		return motor_max_turn_speed;
	}else{
		return motor_mid_turn_speed;
	}
}

char calcError(void){
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
	}else if((7-most_left) == most_right){ //same only one point
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
	return error;
}

void executeControl(void){
    writeServoControl(direction);
    writeMotorControl(motor_speed, motor_dir);
}

void updateLcd(void){
	//show state
    //show error
	//show direction and speed
	return;
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

void writeMotorControl(uint8_t motor_speed){
    if(motor_speed){
        if(!motor_status){
            setMotorStatus(MOTOR_ON);
        }
        setMotorPWM(motor_speed);
    }else{
        if(motor_status){
            setMotorStatus(MOTOR_OFF);
        }
    }
}

void setServoStatus(uint8_t status){
    if(status){
        TCCR1A |= (1<<COM1A1);//enable servo
    }else{
        TCCR1A &= ~(1<<COM1A1);//dissable servo
    }
}

void writeServoControl(char direction){
    if(direction < min_servo_value){
        direction = min_servo_value;
        reportError(SEVO_UNDER_MIN_CONTROLL);
    }else if(direction > max_control_value){
        direction = max_control_value;
        reportError(SERVO_OVER_MAX_CONTROL);
    }
    TCNT1 = 0;
    OCR1A = SERVO_MIN_PULSE + (direction + 90) * PULSE_WIDTH; 
}

void setMotorPWM(uint8_t speed){
	//set motor pwm pulse width
}

void setMotorStatus(uint8_t status){
    motor_status = status;
    if(status){
        //enable motor clock
    }else{
        //dissable motor clock and set motor pins to ground
    }
}
