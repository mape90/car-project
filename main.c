/*-------------------------------------------------------------------------------------*\
* Aalto T-106.5300 Embedded Systems  - Car project 2014                                 *
* project group: Peltola Markus, Suomalainen Janne & Törmänen Olli                      *
*                                                                                       *
* Included files:                                                                       *
* -----------------                                                                     *
*   1) definitions (header) : all defined values (configurations etc)                   *
*   2) includes (header)    : one file for including all headers once                   *
*   3) eventTimer(h+c)      : timer that executes function routines when set time has   *
*                             elapsed                                                   *
*   3) motor(h+c)           : logic for controlling drive motor                         *
*   4) servo(h+c)           : logic for controlling servo (turn) motor                  *
*   5) operationLogic(h+c)  : logic for basic calculations and controls                 *
*   6) UI (h+c)             : User Interface functions (LCD + button)                   *
*   7) Tests (h+c)          : Test functions to test basic functionslity                *
*   8) pid (h+c)            : PID-controller modified from Atmel's PID-library          *
*                                                                                       *
\*-------------------------------------------------------------------------------------*/

#include "includes.h"

/* Function prototypes */

void synchronizeLoopSpeed(void);
void loop(void);
void setup(void);

/* Global variables */
volatile uint8_t gState = STATE_WAIT;
volatile bool gLoopTimeNotElapsed = true;
volatile bool gStarted = false;
volatile bool gIsRunningState = false;

char gPrevPositionError = 0;
int gCurrentRPM = 0;
uint8_t gLCDErrorFlags = 0;
uint8_t gBumperValue = 0;
uint8_t gLapCount = 0;
uint16_t gTachometerValue = 0;
bool gFindRoadTimerElapsed = false;
bool gUICanUpdate = true;
bool gStoppedTextWritten = false;

struct PID_DATA *gPidStMotor;
struct PID_DATA *gPidStServo;

//char gCurrentError = 0;
//int gServoValue = 0;


/* ISR's */

/**
* Event Timer for setting routines that are called after timer value elapses
*/
ISR(TIMER0_OVF_vect)
{
    for(uint8_t i = 0;i < TIMER_MAX_COUNT;i++){
        if(timer_enabled(i))
        {
            if(timer_ended(i))
            {
                timer_disable(i);
                switch(i)
                {
                    case TIMER_1:
                        timer_routine_disableWait();
                        break;
                    case TIMER_2:
                        timer_routine_disableGuiUpdateWait();
                        break;
                    case TIMER_3:
                        timer_routine_changeToRoadNotFoundState();
                        break;
                }
            } else
            {
                timer_update(i);
            }
        }
    }
}


/**
*   ISR for button input
*/
ISR(INT5_vect)
{
	//handleButtonPress();
    if(gIsRunningState)
    {
        // set flag for 'Stop driving' //
        gFindRoadTimerElapsed = true;
    } else
    {
        // First button press starts driving mode //
        gStarted = true;
    }
}

//-----------------------------------//
// Main Program begins               //
//-----------------------------------//
int main(void)
{
    setup();

    LCD_writeString("Ready",8);
    while(!gStarted);

    _delay_ms(500); // to filter button press
    gIsRunningState = true;

    LCD_clear();
    LCD_writeString("Running",8);

    while(1)
    {
        loop(); // (Updated to lastest version)
        synchronizeLoopSpeed();

    }
    return 0;
}
//-----------------------------------//
// Main Program ends                 //
//-----------------------------------//

/* Function implementations */

/***
* Setup function that initializes all operations
*/
void setup(void)
{

    //init timer 0 (Events)
    timer_init();

    //init timer 1 (Servo)
    servo_init();

    //init timer 4 (Motor)
    motor_init();

    //init timer 5 (Tachometer)
    TCCR5B = _BV(ICNC5)| _BV(CS52) | _BV(CS51); //ICNC5 enables filtering
    TCNT5 = 0;
    EIMSK = _BV(INT5);

    // Init PID: Drive Motors //
    gPidStMotor = malloc(sizeof(*gPidStMotor));
    pid_Init(MOTOR_P, MOTOR_I, MOTOR_D, gPidStMotor);
    //gPidStMotor->maxSumError = 10;//MOTOR_I_MAX/ (gPidStMotor->I_Factor + 1);

    // Init PID: Servo Motors //
    gPidStServo = malloc(sizeof(*gPidStServo));
    pid_Init(SERVO_P, SERVO_I, SERVO_D, gPidStServo);
    gPidStServo->maxSumError = SERVO_I_MAX/ (gPidStServo->I_Factor + 1);

    servo_writeControl(0);

    // Init LCD //
    USART_LCD_Init(MYUBRR);

    // Enable interrupts //
    sei();

}

/**
*   Control loop functionality
*/
void loop(void)
{
    if(!gFindRoadTimerElapsed)
    {
        int angle, speed;
        int8_t positionError = calcPositionError(bumper_read());
        control_calc(positionError, &speed, &angle);
        control_execute(speed, angle);
        gPrevPositionError = positionError;
    } else
    // Stop the car //
    {
        motor_writePWM(0);
        if(!gStoppedTextWritten)
        {
            gStoppedTextWritten = true;
            LCD_clear();
            LCD_writeString("      Stopped",8);
        }
    }
	/* Old implementation
	if(gState == STATE_RUNNING || gState == STATE_FINDING_ROAD){
		runCar();
	}else if(gState == STATE_ROAD_NOT_FOUND){
		stopCar();
	}else{ //wait_state
        stopCar();
    }
    LCD_update();
    */
}


/**
* Synchronizes loop speed to set value
* @configuration = LOOP_TIME_MS
*/
void synchronizeLoopSpeed(void)
{
	while(gLoopTimeNotElapsed)
	{
        bumper_calcValue();
	}

	gLoopTimeNotElapsed = true;
	timer_enable(TIMER_1, LOOP_TIME_MS);
}
