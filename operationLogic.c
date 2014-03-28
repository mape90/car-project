#include "includes.h"

/* External Global variables */
extern volatile uint8_t gState;
extern uint8_t gLapCount;
extern int8_t gLastError;
extern uint8_t gBumperValue;
extern bool gFindRoadTimerElapsed;
extern struct PID_DATA *gPidStServo;


/* Internal global variables */
static volatile uint8_t gPreviousState = STATE_WAIT;
static int gIntegerSum = 0;
static int8_t lastError = 0;

void runCar(void)
{
    int angle, speed;
    int8_t error = calcError(readBumper());
    setState(error);
    calcControl(error, &speed, &angle);
    executeControl(speed, angle);
    gLastError = error;
}

void stopCar(void)
{
    executeControl(0, 0);
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
    timer_enable(TIMER_3, ROAD_SEARCH_TIME_MS); //set timer on if it isnt running
}



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

inline uint8_t readBumper(void)
{
    return gBumperValue; //read sensor port data
}

int calcMotorSpeed(int8_t error)
{
    if (error > -8 && error < 8)
        return MOTOR_SPEED_MAX_RPM - abs(error)*MOTOR_SPEED_REDUCE;
    else
        return 0;
}

static bool argh = true;

void calcControl(int8_t error, int* speed, int* angle)
{
	if(error == CONTROL_NO_REF_POINT)
	{
        if(argh){
            setFindTimer();
            argh = false;
        }
        
		//turn same direction where last time turned
        if(lastError >= 0){
            *angle = pid_Controller(0, 7, gPidStServo);}
        else{
            *angle = pid_Controller(0, -7, gPidStServo);}
        *speed = calcMotorSpeed(7);
	}
	else if(error == GOAL_POINT)
	{
        if(!argh){timer_disable(TIMER_3);argh = true;}
        *angle = pid_Controller(0, lastError, gPidStServo);
        *speed = calcMotorSpeed(0);
	}
	else
	{
        if(!argh){timer_disable(TIMER_3);argh = true;}
		*angle = pid_Controller(0, error, gPidStServo);
		*speed = calcMotorSpeed(error);
	}
}

int8_t calcError(uint8_t sensorValues)
{
    
    int8_t error = 0;
    int8_t mostLeft = 20;
    int8_t mostRight = 20;

    for(int8_t i = 0;i < 8;i++){
	    if(sensorValues & (0x80 >> i)){
		    mostLeft = i;
		    break;
	    }
    }
    for(int8_t i = 0;i < 8;i++){
	    if(sensorValues & (0x01 << i)){
		    mostRight = i;
		    break;
	    }
    }

    if(mostLeft == 20 || mostRight == 20){//no reference point found out of track
      if(lastError < 7 && lastError > -7){
        error = lastError;
      }else{
        error = CONTROL_NO_REF_POINT;
      }
    }else if(abs((7 - mostLeft) - mostRight) > GOAL_MIN_WIDTH){
      error = GOAL_POINT;
    }else{
        error = (int8_t)7 - (int8_t)2*mostRight;
        lastError = error;
    }

    //LCD_Write_int(error,6);

    return error;
}

void executeControl(int speed, int angle)
{
    writeServoControl(angle);//Servo
    setMotorSpeed(speed);//motor
}
