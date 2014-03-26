#ifndef DEFINITIONS_HH
#define DEFINITIONS_HH

/* Settings */

#define LCD_REFRESH_RATE_MS             500
#define TICKS_PER_ROTATION              2
#define ROAD_SEARCH_TIME_MS             4000
#define MAX_LAP_COUNT                   1
#define LOOP_TIME_MS                    100
#define GOAL_MIN_WIDTH                  2
#define GOAL_POINT                      100
#define P                               1
#define I                               0
#define D                               0

#define MOTOR_SPEED_MAX_RPM             1000
#define ROAD_FIND_SPEED_RPM             (MOTOR_SPEED_MAX_RPM/2)

#define MOTOR_SPEED_REDUCE              MOTOR_SPEED_MAX_RPM/9

#define MOTOR_CONTROL_MAX               (65536/3)
#define MOTOR_ACC_MAX                   1000
#define MOTOR_I_VALUE_MAX               MOTOR_CONTROL_MAX

#define MOTOR_P                         ((uint8_t)1)
#define MOTOR_I                         ((uint8_t)0)

/* Port / pin -settings */
#define BUMPER_REGISTER                 PINA       // Todo: set this to actual reg.
#define SERVO_PIN                       PB5
#define MOTOR_PIN_PWM                   PH3
#define MOTOR_PIN_INA                   PK0
#define MOTOR_PIN_INB                   PK1
#define MOTOR_PIN_ENA                   PK2
#define MOTOR_PIN_ENB                   PK3

#define LEDS_PORT                       PORTC
#define LEDS_DDR                        DDRC
#define LEDS_PIN                        PINC

#define BUTTONS_PORT                    PORTA
#define BUTTONS_DDR                     DDRA
#define BUTTONS_PIN                     PINA

/* Pre-defined values */
#define CONTROL_NO_REF_POINT            101

#define MOTOR_BACKWARD                  0
#define MOTOR_FORWARD                   1
#define MOTOR_ON                        ((uint8_t)1)
#define MOTOR_OFF                       ((uint8_t)0)
#define MOTOR_NO_REF_SPEED              ((char)-120)

#define SERVO_ON                        ((uint8_t)1)
#define SERVO_OFF                       ((uint8_t)0)
#define SERVO_CONTROL_VALUE_MAX         90

#define CONTROL_VALUE_MAX               255
#define INTEGER_MAX                     200

#define SERVO_VALUE_MAX                 ((int)45)
#define SERVO_VALUE_MIN                 ((int)-45)
#define SERVO_PULSE_MIN                 ((int)2000)
#define SERVO_PULSE_WIDTH               ((int)11)

#define STATE_WAIT                      1
#define STATE_RUNNING                   2
#define STATE_FINDING_ROAD              3
#define STATE_ROAD_NOT_FOUND            4

#define TIMER_MAX_COUNT                 3
#define TIMER_1                         ((uint8_t)0)
#define TIMER_2                         ((uint8_t)1)
#define TIMER_3                         ((uint8_t)2)

/* LCD values */
#define MYUBRR 103

#define AUTO_BAUD 0x55
#define RECEIVED 0x06
#define DRAW_STRING 0x73
#define COLUMN_0 0x00
#define ROW_1 0x01
#define ROW_2 0x02
#define ROW_3 0x03
#define ROW_5 0x05

#define FONT 0x03
#define WHITE 0xFF
#define CLEAR 0x45

/* Error codes */
#define ERR_SERVO_UNDER_MIN_CONTROL     10
#define ERR_SERVO_OVER_MAX_CONTROL      11

uint8_t bumber_buffer[BUMPER_BUFFER_SIZE];

uint8_t filter_values[] = {0,0,0,0,0,0,0,0};
for(uint8_t j = 0; j < BUMPER_BUFFER_SIZE;j++)
    for(uint8_t i = 0;i < 8;i++)
        filter_values[] += val[j] & (1<<i);

filtered = 0;
for(uint8_t i = 0;i < 8;i++)
    if(filter_values[i] > BUMPER_BUFFER_SIZE/2)
        filtered |= (1<<i;

#endif
