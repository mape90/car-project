#include "includes.h"

/* External Global variables */
extern volatile uint8_t gState;
extern uint8_t gLapCount;
extern int8_t gPrevPositionError;
extern uint8_t gBumperValue;
extern bool gFindRoadTimerElapsed;
extern struct PID_DATA *gPidStServo;


/* Internal global variables */
static volatile uint8_t iPreviousState = STATE_WAIT;
static int8_t iLastPositionErrorValue = 0;
static bool iSearchTimerNotSet = true;
//static int gIntegerSum = 0;


/* Function implementations */

/* not in use
void runCar(void)
{
    int angle, speed;
    int8_t error = calcError(readBumper());
    setState(error);
    calcControl(error, &speed, &angle);
    executeControl(speed, angle);
    gLastError = error;
}
*/
/* not in use
void stopCar(void)
{
    executeControl(0, 0);
}
*/

/* not in use
void setNewState(uint8_t newState)
{
	uint8_t temp = SREG;
	cli();
	gPreviousState = gState;
	gState = newState;
	SREG = temp;
}

*/



/* not in use
void setState(int8_t error)
{
	if(error == GOAL_POINT)
	{
		if(error != gLastError)
		{
			gLapCount++;
			if(gLapCount >= MAX_LAP_COUNT)
			{
				setNewState(STATE_WAIT);
				gLapCount=0;
			}
		}
	}else if(error == CONTROL_NO_REF_POINT)
	{
        if (gState == STATE_FINDING_ROAD)
        {
            if (gFindRoadTimerElapsed)
            {
                setNewState(STATE_ROAD_NOT_FOUND);
            }
        }
        else
        {
            setNewState(STATE_FINDING_ROAD);
            setFindTimer();
        }

	}else
	{
        if(gState != STATE_RUNNING)
        {
            setNewState(STATE_RUNNING);
        }
	}
}
*/

/**
    Enable timer for searching track before giving up and stopping
    the race. Timer value (ms) is determined by ROAD_SEARCH_TIME_MS.
*/
void setFindTimer()
{
    timer_enable(TIMER_3, ROAD_SEARCH_TIME_MS); //set timer on if it isnt running
}

/**
* Calculate & update bumper value based on values in read buffer
* This function must be executed as many times as the value BUMPER_READ_BUFF_SIZE  is
* before it will calculate mode value for each bumper bit in register
*/
void bumper_calcValue(void)
{
    static uint8_t bumperBuffer[BUMPER_READ_BUFF_SIZE];
    static unsigned int buffCursor = 0;
    static bool initBuffer = true;
    static unsigned int valCounter  = 0;

    uint8_t bumperVal;
    if (initBuffer)
    {
        initBuffer = false;
        for (int i = 0; i < BUMPER_READ_BUFF_SIZE; i++)
            bumperBuffer[i] = 0;
    }

    bumperVal = ~BUMPER_REGISTER;
    //LCD_Write_int((int)bumperVal, 7);

    //if (isValidBumperValue(bumperVal))

    bumperBuffer[buffCursor++] = bumperVal;
    valCounter++;

    if (buffCursor >= BUMPER_READ_BUFF_SIZE)
        buffCursor = 0;

    if (valCounter >= BUMPER_READ_BUFF_SIZE)
    {
        valCounter = 0;
        // Calculate single "average" value from buffer //

        uint8_t filteredValue = 0;
        uint8_t filter_values[] = {0,0,0,0,0,0,0,0};
        for(uint8_t j = 0; j < BUMPER_READ_BUFF_SIZE;j++)
        {
            for(uint8_t i = 0; i < 8; i++)
                filter_values[i] += ((bumperBuffer[j] & (1<<i)) ? 1: 0);
        }

        for(uint8_t i = 0;i < 8;i++)
        {
            if(filter_values[i] > (BUMPER_READ_BUFF_SIZE/2)){
                filteredValue |= (1<<i);
            }
        }
        gBumperValue = filteredValue;
    }
}


/**
    Encapsulation for filtered bumper value
*/
inline uint8_t bumper_read(void)
{
    return gBumperValue; //read sensor port data
}

/**
    Calculate parameter values for speed and angle based on
    error value.
*/
void control_calcParameters(int8_t error, int* speed, int* angle)
{
	if(error == CONTROL_NO_REF_POINT)
	// Track is lost -> Track search mode //
	{
        if(iSearchTimerNotSet)
        // Start search timer for track searching //
        {
            setFindTimer();
            iSearchTimerNotSet = false;
        }

		// continue turning to the  same direction where last time was turned
        if(iLastPositionErrorValue >= 0)
        {
            *angle = pid_Controller(0, 7, gPidStServo);
        }
        else
        {
            *angle = pid_Controller(0, -7, gPidStServo);
        }

        *speed = motor_calcSpeed(MOTOR_ERROR_ON_SEARCH);
	}
	else if(error == GOAL_POINT)
	// Car is on top of finish-line //
	{
        if(!iSearchTimerNotSet)
        {
            timer_disable(TIMER_3);
            iSearchTimerNotSet = true;
        }
        *angle = pid_Controller(0, iLastPositionErrorValue, gPidStServo);
        *speed = motor_calcSpeed(0);
	}
	else
    // Normal driving situation //
	{
        if(!iSearchTimerNotSet)
        // Back on track -> Disable search timer //
        {
            timer_disable(TIMER_3);
            iSearchTimerNotSet = true;
        }

        // Calculate preferable parameter values //
		*angle = pid_Controller(0, error, gPidStServo);
		*speed = motor_calcSpeed(error);
	}
}

/**
    Calculate position error from front sensor values.
    Returns error value [-7, 7] or defined GOAL_POINT or
    CONTROL_NO_REF_POINT.
*/
int8_t control_calcPositionError(uint8_t sensorValues)
{
    int8_t error = 0;
    int8_t mostLeft = 20;
    int8_t mostRight = 20;

    // Check sensor readings for a line detection starting from the outer side
    for(int8_t i = 0;i < 8;i++)
    {
	    if(sensorValues & (0x80 >> i))
        {
		    mostLeft = i;
		    break;
	    }
    }
    for(int8_t i = 0;i < 8;i++)
    {
	    if(sensorValues & (0x01 << i))
	    {
		    mostRight = i;
		    break;
        }
    }

    if(mostLeft == 20 || mostRight == 20)
    //no reference point found out of track //
    {
      if(iLastPositionErrorValue < 7 && iLastPositionErrorValue > -7)
      // Previous detection was from one of the inner sensors
      {
        // use previous value //
        error = iLastPositionErrorValue;
      }
      else
      // Previous detection was from one of the outer sensors -> track lost case
      {
        error = CONTROL_NO_REF_POINT;
      }
    }
    else if (abs((7 - mostLeft) - mostRight) > GOAL_MIN_WIDTH)
    // New value found: finish line detected  //
    {
        error = GOAL_POINT;
    }
    else
    // New value found: calculate new error value
    {
        error = (int8_t)7 - (int8_t)2*mostRight;
        iLastPositionErrorValue = error;
    }

    //LCD_Write_int(error,6);
    return error;
}

/**
    Executes new driving commands (speed and angle) based on
    given values.
*/
void control_execute(int speed, int angle)
{
    servo_writeControl(angle);//Servo
    motor_setSpeed(speed);//motor
}
