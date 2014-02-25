#include "includes.h"

/* External Global variables */
extern volatile uint8_t gState;
extern uint8_t gLapCount;
extern char gLastError;

/* Internal global variables */
volatile static uint8_t gPreviousState = STATE_WAIT;
static int gIntegerSum = 0;


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

void setFindTimer(){
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
		setNewState(STATE_FINDING_ROAD);
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
    // Todo: Implement this
    return error;
}

void calcControl(uint8_t error, char speed, uint8_t angle)
{
	if(error == CONTROL_NO_REF_POINT)
	{
		//do what is needed to do when track is lost
        angle = pidValue2Deg(PID(0));
        speed = calcMotorSpeed(MOTOR_NO_REF_SPEED);//TODO
	}
	else if(error == GOAL_POINT)
	{
        angle = pidValue2Deg(PID(0));
        speed = calcMotorSpeed(0);
    }
    else
    {
		angle = pidValue2Deg(PID(error));
		speed = calcMotorSpeed(error);
	}
}

char pidValue2Deg(char val)
{
	return val;
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

void executeControl(char speed, uint8_t angle)
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
