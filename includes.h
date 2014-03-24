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

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 0

//CONSTANT VALUES

//Global

const uint8_t loop_time_ms = 100;
const uint16_t finding_road_time_ms = 1000;

const uint8_t goal_min_wide = 2;
const uint8_t timer_max_count = 3;
const uint8_t timer_1 = 0;
const uint8_t timer_2 = 1;
const uint8_t timer_3 = 2;
const uint8_t max_lap_count = 1;

const char find_road_speed = -10;

const char no_reference_speed = -120;
const uint8_t no_reference_point = 101; 
const uint8_t goal_point = 100;


//Servo
const char max_servo_value = 90;
const char min_servo_value = -90;

const uint8_t servo_pin = 3;

const uint16_t servo_min_pulse 2000;

//Motor
const uint8_t motor_pin_1 = ;
const uint8_t motor_pin_2 = ;
const uint8_t integer_max = 200;
const uint8_t max_control_value = 255;

const uint8_t P_motor = 1;
const uint8_t I_motor = 0;

const uint16_t max_motor_controll = 65536/3;
const uint16_t max_motor_I_value = max_motor_controll;

const uint8_t max_motor_acc = 10;
const uint8_t tacho_buffer_size = 10;

const uint16_t max_speed = 1000; //RPM
const uint8_t speed_reduce = max_speed / 9;

//Comtroll

const uint8_t wait_state = 1;
const uint8_t running_state = 2;
const uint8_t finding_road_state = 3;
const uint8_t road_not_found = 4;

const uint8_t P = 1;
const uint8_t I = 0;
const uint8_t D = 0;

//LCD

const uint8_t lcd_wait_time = 100;
const char road_not_found_error_msg[] = "Road not found";


//GLOBAL VARIABLES

volatile uint8_t state = wait_state;
volatile uint8_t previous_state = wait_state;
volatile uint8_t need_waiting = true;
volatile char wheel_orientation = 0;
volatile char error_current = 0;
volatile char last_error = 0;
volatile uint16_t tacho_val = 0;
volatile int current_rpm = 0;
volatile int last_pwm = 0;

uint8_t lap_count = 0;
uint8_t tacho_buffer_cursor = 0;

uint8_t gui_can_update = false;
uint8_t find_road_timer_out_of_time = false;
uint8_t has_error = false;

uint16_t timers_current_value[timer_count];
uint16_t timer_set_value[timer_count];
uint16_t buffer[tacho_buffer_size] = {0,0,0,0,0,0,0,0,0,0};

int integer_sum = 0;
int servo_value = 0;

#endif