/********************************
 * Settings and defined values  *
 ********************************/

#ifndef DEFINITIONS_HH
#define DEFINITIONS_HH

/* Settings */

#define LCD_REFRESH_RATE_MS             500

#define TICKS_PER_ROTATION              2
#define ROAD_SEARCH_TIME_MS             4000
#define MAX_LAP_COUNT                   1
#define LOOP_TIME_MS                    3
#define GOAL_MIN_WIDTH                  1

#define GOAL_POINT                      ((int8_t)100)
#define CONTROL_NO_REF_POINT            ((int8_t)101)

//#define CONTROL_VALUE_MAX             255


#define BUMPER_READ_BUFF_SIZE           4
#define TACHOMETER_BUFFER_SIZE          4

#define ROAD_FIND_SPEED_RPM             (MOTOR_SPEED_MAX_RPM/2)

//MOTOR

#define MOTOR_SCALE_RPM                 10

#define MOTOR_CONTROL_MAX               ((int)(0xffff/3))
#define MOTOR_CONTROL_MIN               (int)-1000


#define MOTOR_SPEED_MAX_RPM             (int16_t)750 //850
#define MOTOR_ACC_MAX                   (int16_t)10000
#define MOTOR_I_MAX                     50  //max value

#define MOTOR_SPEED_REDUCE              13
#define MOTOR_ERROR_ON_SEARCH           7

#define MOTOR_P                         3000 //3080
#define MOTOR_I                         10
#define MOTOR_D                         50


//SERVO

#define SERVO_I_MAX                     15*128

#define SERVO_P                         1100
#define SERVO_I                         10
#define SERVO_D                         20

#define SERVO_CONTROL_VALUE_MAX         ((int8_t)70)
#define SERVO_VALUE_MAX                 SERVO_CONTROL_VALUE_MAX
#define SERVO_VALUE_MIN                 -SERVO_CONTROL_VALUE_MAX
#define SERVO_PULSE_MIN                 ((int)2000)
#define SERVO_PULSE_WIDTH               ((int)11)


/* Port / pin -settings */
#define BUMPER_REGISTER                 PINA
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


#define MOTOR_BACKWARD                  ((uint8_t)0)
#define MOTOR_FORWARD                   ((uint8_t)1)
#define MOTOR_ON                        ((uint8_t)1)
#define MOTOR_OFF                       ((uint8_t)0)
#define MOTOR_NO_REF_SPEED              ((char)-120)

#define SERVO_ON                        ((uint8_t)1)
#define SERVO_OFF                       ((uint8_t)0)


#define STATE_WAIT                      ((uint8_t)1)
#define STATE_RUNNING                   ((uint8_t)2)
#define STATE_FINDING_ROAD              ((uint8_t)3)
#define STATE_ROAD_NOT_FOUND            ((uint8_t)4)

#define TIMER_MAX_COUNT                 3
#define TIMER_1                         ((uint8_t)0)
#define TIMER_2                         ((uint8_t)1)
#define TIMER_3                         ((uint8_t)2)

/* LCD values */
#define MYUBRR                          103

#define AUTO_BAUD                       0x55
#define RECEIVED                        0x06
#define DRAW_STRING                     0x73
#define COLUMN_0                        0x00
#define ROW_1                           0x01
#define ROW_2                           0x02
#define ROW_3                           0x03
#define ROW_5                           0x05

#define FONT                            0x03
#define WHITE                           0xFF
#define CLEAR                           0x45

/* Error codes */
#define ERR_SERVO_UNDER_MIN_CONTROL     10
#define ERR_SERVO_OVER_MAX_CONTROL      11

#endif
