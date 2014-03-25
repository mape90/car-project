#include "includes.h"

/* External Global variables */
extern volatile uint8_t gState;
extern uint8_t gLapCount;
extern char gLastError;

extern bool gFindRoadTimerElapsed;

/* Internal global variables */
static volatile uint8_t gPreviousState = STATE_WAIT;
static int gIntegerSum = 0;

void runCar(void)
{
    uint8_t angle;
    char speed, error = calcError(readBumper());
    setState(error);
    calcControl(error, &speed, &angle);
    executeControl(speed, angle);
    gLastError = error;
}

void stopCar(void)
{
    executeControl(0, 0);
}

void findRoad(void)
{
    executeControl(ROAD_FIND_SPEED_RPM, 0); //TODO make more intelligent
}


void setNewState(uint8_t newState)
{
	uint8_t temp = SREG;
	cli();
	gPreviousState = gState;
	gState = newState;
	SREG = temp;
}

void handleFindTimer(){
	setNewState(STATE_ROAD_NOT_FOUND);
}

void setFindTimer()
{
    timer_enable(TIMER_3, ROAD_SEARCH_TIME_MS);
	//set timer on if it isnt running
}



void setState(char error)
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

inline uint8_t readBumper(void)
{
    return BUMPER_REGISTER; //read sensor port data
}

char calcMotorSpeed(char error)
{
    if (error > -8 && error < 8)
        return MOTOR_SPEED_MAX_RPM - abs(error)*MOTOR_SPEED_REDUCE;

    else
        return 0;
}

void calcControl(uint8_t error, char* speed, uint8_t* angle)
{
	if(error == CONTROL_NO_REF_POINT)
	{
		//do what is needed to do when track is lost
        *angle = PID(0);
        *speed = calcMotorSpeed(MOTOR_NO_REF_SPEED);//TODO
	}
	else if(error == GOAL_POINT)
	{
        *angle = PID(0);
        *speed = calcMotorSpeed(0);
    }
    else
    {
		*angle = PID(error);
		*speed = calcMotorSpeed(error);
	}
}

char calcError(uint8_t sensorValues)
{
	char error = 0;
	char mostLeft = -1;
	char mostRight = -1;

	for(uint8_t i = 0;i < 8;i++)
	{
		if(sensorValues & (0x80 >> i))
		{
			mostLeft = i;
			break;
		}
	}
	for(uint8_t i = 0;i < 8;i++)
	{
		if(sensorValues & (0x01 << i))
		{
			mostRight = i;
			break;
		}
	}
	if(mostLeft < 0 || mostRight < 0)
	{//no reference point found out of track
		error = CONTROL_NO_REF_POINT;
	}
	else if(abs((7 - mostLeft) - mostRight) > GOAL_MIN_WIDTH)
	{
        for(uint8_t i = 1; i < GOAL_MIN_WIDTH; i++)
        {
            if(!(sensorValues & (0x01 << (mostRight+i))))
            {
                error = 1;
                break;
            }
        }
        if(error)
        {
            error = GOAL_POINT;
        }else
        {
            error = 0;
        }
    }
    if(error == 0)
    {
        if((7 - mostLeft) == mostRight)
        { //same only one point
            error = 7-2*mostRight;
        }
        else if(mostLeft < mostRight)
        { //use left as controll
            if(sensorValues & (0x80 >> (mostLeft + 1)) )
            { // chek if line is between sensors
                error = -6 + 2 * mostLeft;
            }
            else
            {
                error = -7 + 2 * mostLeft;
            }
        }
        else
        { //use right as controll
            if(sensorValues & (0x01 << (mostRight + 1)) )
            { // chek if line is between sensors
                error = 6 - 2 * mostRight;
            }
            else
            {
                error = 6 - 2 * mostRight;
            }
        }
    }

	return error;
}

void executeControl(int speed, uint8_t angle)
{
    writeServoControl(angle);//Servo
    setMotorSpeed(speed);//motor
}

int PID(char error)
{
    int control = P * error;
    gIntegerSum += I*error;

    if(abs(gIntegerSum) > INTEGER_MAX)
    {//integer wind up
        gIntegerSum = (gIntegerSum < 0) ? -1* INTEGER_MAX : INTEGER_MAX;
    }

    control += D * (error - gLastError) + gIntegerSum;

    if(abs(control) > CONTROL_VALUE_MAX)
    {//limit control values
        control = (control < 0) ? -1*CONTROL_VALUE_MAX : CONTROL_VALUE_MAX;
    }

    return control;
}
