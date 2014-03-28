#include "includes.h"

/* Function prototypes */

void synchronizeLoopSpeed(void);
void loop(void);
void setup(void);

/* Global variables */
volatile uint8_t gState = STATE_WAIT;
volatile bool gLoopTimeNotElapsed = true;

char gCurrentError = 0;
char gLastError = 0;
int gServoValue = 0;
int gCurrentRPM = 0;
uint8_t gLapCount = 0;
uint16_t gTachometerValue = 0;
bool gFindRoadTimerElapsed = false;
bool gUICanUpdate = true;
uint8_t gLCDErrorFlags = 0;
uint8_t gBumperValue = 0;
struct PID_DATA *gPidStMotor;

struct PID_DATA *gPidStServo;

/* ISR's */
ISR(TIMER0_OVF_vect)
{
    for(uint8_t i = 0;i < TIMER_MAX_COUNT;i++){
        if(timer_enabled(i)){
            if(timer_ended(i)){
                timer_disable(i);
                switch(i){
                    case TIMER_1:
                        disable_wait();
                        break;
                    case TIMER_2:
                        disableGuiUpdateWait();
                        break;
                    case TIMER_3:
                        changeToRoadNotFoundState();
                        break;
                }
            }else{
                timer_update(i);
            }
        }
    }
}

ISR(INT5_vect){
	handleButtonPress();
}

//-----------------------------------//

int main(void)
{
    setup();

    LCD_Write_String("test start",8);
    _delay_ms(1000);
    while(1)
    {

        //writeMotorPWM(1000);
        //test_servo_loop();
        //test_motor_loop();
        //test_controll_loop();
        //test_tachometer_PI_loop();
        loop();
        //LCD_Write_int((int)gBumperValue, 7);
        synchronizeLoopSpeed();

    }
    return 0;
}

void setup(void) {

    //init timer 0 (Events)
    initEventTimer();

    //init timer 1 (Servo)
    servo_init();

    //init timer 4 (Motor)
    motor_init();

    //init timer 5 (Tachometer)
    TCCR5B = _BV(ICNC5)| _BV(CS52) | _BV(CS51); //ICNC5 enables filtering
    TCNT5 = 0;

    EIMSK = _BV(INT5);

    gPidStMotor = malloc(sizeof(*gPidStMotor));
     
    pid_Init(MOTOR_P, MOTOR_I, MOTOR_D, gPidStMotor);
//gPidStMotor->maxSumError = 10;//MOTOR_I_MAX/ (gPidStMotor->I_Factor + 1);

    gPidStServo = malloc(sizeof(*gPidStServo));
    gPidStServo->maxSumError = SERVO_I_MAX/ (gPidStServo->I_Factor + 1);
    pid_Init(SERVO_P, SERVO_I, SERVO_D, gPidStServo);

    writeServoControl(0);
    USART_LCD_Init(MYUBRR);
    sei();

}

void loop(void){
	if(gState == STATE_RUNNING || gState == STATE_FINDING_ROAD){
		runCar();
	}else if(gState == STATE_ROAD_NOT_FOUND){
		stopCar();
	}else{ //wait_state
        stopCar();
    }
    LCD_update();
}


void synchronizeLoopSpeed(void)
{
	while(gLoopTimeNotElapsed)
	{
        calcBumperValue();
	}

	gLoopTimeNotElapsed = true;
	timer_enable(TIMER_1, LOOP_TIME_MS);
}
