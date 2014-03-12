#ifndef INCLUDES__HH
#define INCLUDES__HH

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

const uint8_t no_reference_point = 101; 
const uint8_t goal_point = 100;

const uint8_t motor_straight_speed = 255/3;
const uint8_t motor_mid_turn_speed = 150;
const uint8_t motor_max_turn_speed = 100;


const uint8_t wait_state = 1;
const uint8_t running_state = 2;
const uint8_t finding_road_state = 3;
const uint8_t road_not_found = 4;

const uint8_t loop_time_ms = 100;

const uint8_t goal_min_wide 2;

//GLOBAL VARIABLES

volatile uint8_t state = wait_state;
volatile uint8_t previous_state = wait_state;

char error_current = 0;
char last_error = 0;

int integer_sum = 0;

int servo_value = 0;
uint8_t motor_speed = 0;

volatile uint8_t need_waiting = true;

const uint8_t timer_max_count = 3;
const uint8_t timer_1 = 0;
const uint8_t timer_2 = 1;
const uint8_t timer_3 = 2;
uint8_t lap_count = 0;
const uint8_t max_lap_count = 1;

uint16_t timers_current_value[timer_count];
uint16_t timer_set_value[timer_count];


#endif