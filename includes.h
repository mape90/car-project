#ifndef INCLUDES__HH
#define INCLUDES__HH

/* Included files */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>

#include "operationLogic.h"
#include "motor.h"
#include "servo.h"
#include "UI.h"
#include "eventTimer.h"

/* Settings */
#define LOOP_TIME_MS 100
#define GOAL_MIN_WIDTH 2
#define GOAL_POINT 100
#define P 1
#define I 0
#define D 0

#define MOTOR_SPEED_STRAIGHT
const uint8_t motor_straight_speed = 255/3;
const uint8_t motor_mid_turn_speed = 150;
const uint8_t motor_max_turn_speed = 100;






/* Port / pin -settings */
#define BUMPER_REGISTER 0   // Todo: set this to actual reg.
#define SERVO_PIN 3
#define MOTOR_PIN_1 0       // Todo: set this to actual pin.
#define MOTOR_PIN_2 0       // Todo: set this to actual pin.

#define LEDS_PORT PORTC
#define LEDS_DDR  DDRC
#define LEDS_PIN  PINC

#define BUTTONS_PORT PORTA
#define BUTTONS_DDR  DDRA
#define BUTTONS_PIN  PINA

/* Pre-defined values */
#define CONTROL_NO_REF_POINT 101
#define MOTOR_ON 1
#define MOTOR_OFF 0
#define MOTOR_NO_REF_SPEED (char) -120

#define SERVO_ON 1
#define SERVO_OFF 0

#define CONTROL_VALUE_MAX 255
#define INTEGER_MAX 200

#define SERVO_VALUE_MAX (char) 90
#define SERVO_VALUE_MIN (char) -90
#define SERVO_PULSE_MIN (uint16_t) 2000
#define SERVO_PULSE_WIDTH 11

#define STATE_WAIT 1
#define STATE_RUNNING 2
#define STATE_FINDING_ROAD 3
#define STATE_ROAD_NOT_FOUND 4

/* Error codes */
#define ERR_SERVO_UNDER_MIN_CONTROL 10
#define ERR_SERVO_OVER_MAX_CONTROL 11

//CONSTANT VALUES




//GLOBAL VARIABLES





char error_current = 0;
char last_error = 0;

int servo_value = 0;
uint8_t motor_speed = 0;



#define TIMER_MAX_COUNT 3
#define TIMER_1 (uint8_t) 0
#define TIMER_2 (uint8_t) 1
#define TIMER_3 (uint8_t) 2

uint8_t lap_count = 0;
const uint8_t max_lap_count = 1;



#define MOTOR_CONTROL_MAX 65536/3
const uint16_t max_motor_I_value = MOTOR_CONTROL_MAX;
const uint8_t P_motor = 1;
const uint8_t I_motor = 0;

#endif
