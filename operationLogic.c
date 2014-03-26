#include "includes.h"

/* External Global variables */
extern volatile uint8_t gState;
extern uint8_t gLapCount;
extern char gLastError;
extern uint8_t gBumperValue;
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

bool isValidBumperValue(uint8_t bumperVal)
{
    // Accepter number of hits: >= 6 or <=2
    unsigned int counter = 0;
    uint8_t itr = 0x01;
    while (itr <= 0x80)
    {
        if (itr & bumperVal)
            counter++;
        itr = (itr << 1);
    }

    return (counter >= 6 || counter <= 2);
}
void calcBumperValue(void)
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

    bumperVal = BUMPER_REGISTER;
    //if (isValidBumperValue(bumperVal))
    {
        bumperBuffer[buffCursor++] = bumperVal;
        valCounter++;
    }
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
            for(uint8_t i = 0; i < sizeof(uint8_t); i++)
                filter_values[i] += bumperBuffer[j] & (1<<i);
        }

        for(uint8_t i = 0;i < 8;i++)
        {
            if(filter_values[i] > BUMPER_READ_BUFF_SIZE/2)
                filteredValue |= (1<<i);
        }
        gBumperValue = filteredValue;
    }
}

inline uint8_t readBumper(void)
{
    return gBumperValue; //read sensor port data
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
	static char lastError = 0;
	char error = 0;
	char mostLeft = -1;
	char mostRight = -1;


	for(uint8_t i = 0;i < 8;i++)
	{
        //LCD_clear();
        //_delay_ms(400);
		//LCD_Write_int(i);
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
        if(lastError < 7){
            error = lastError + ((lastError < 0) ? 1: -1);
        }else{
            error = CONTROL_NO_REF_POINT;
        }
	}else if(abs((7 - mostLeft) - mostRight) > GOAL_MIN_WIDTH)
	{
        error = GOAL_POINT;
    }else{
        error = 7-2*mostRight;
        lastError = error;
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
