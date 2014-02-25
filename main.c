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
uint8_t gMotorSpeed = 0;
uint8_t gLapCount = 0;

/* ISR's */
ISR(INT5_vect)
{
	handleButtonPress();
}

ISR(TIMER0_OVF_vect)
{
    for(uint8_t i = 0;i < TIMER_MAX_COUNT;i++)
    {
        if(timer_getValue(i) > 0)
        {
            if(timer_getCurrentValue(i) > timer_getValue(i))
            {
                timer_setCurrentValue(i, 0);
                timer_setValue(i, 0);
                switch(i)
                {
                    case TIMER_1:
                        disable_wait();
                        break;
                    case TIMER_2:
                        timer_2_function();
                        break;
                    case TIMER_3:
                        timer_3_function();
                        break;
                }
            }else
            {
                timer_setCurrentValue(i, timer_getCurrentValue(i) + 1);
            }
        }
    }
}

int main(void) {
    setup();

    while(1)
    {
		loop();
		synchronizeLoopSpeed();
    }

    return 0;
}

void setup(void) {

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

    initEventTimer();

    writeServoControl(0);
    sei();
}

void loop(void)
{
	if(gState == STATE_RUNNING || gState == STATE_FINDING_ROAD)
	{
        // Todo: init speed & angle
        char speed = 0;
        uint8_t angle = 0;
		char error = calcError(readBumper());

		setState(error);
		calcControl(error, speed, angle);
		executeControl(speed, angle);
		gLastError = error;
	}else
	{
		//wait_state, road_not_found
		//do nothing
	}

    updateLcd();
}

void synchronizeLoopSpeed(void)
{
	while(gLoopTimeNotElapsed);

	gLoopTimeNotElapsed = true;
	timer_setValue(TIMER_1, LOOP_TIME_MS);
}
