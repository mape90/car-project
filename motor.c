#include "motor.h"
#include "includes.h"

/* External Global Variables */
extern uint16_t gTachometerValue;
extern int gCurrentRPM;

void motor_init(void)
{
    DDRH |= (1 << MOTOR_PIN_PWM);
    TCCR4A = (1 << COM4A1) | (1 << WGM41); // Fast PWM
    TCCR4B |= (1 << CS40) | (1 << WGM42) | (1 << WGM43); //
    ICR4 = 0xffff;
    OCR4A = 0; //set motor speed to zero
    DDRK |= (1 << MOTOR_PIN_INA) | (1 << MOTOR_PIN_INB); //INA and INB
    DDRK &= ~((1 << MOTOR_PIN_ENA) | (1 << MOTOR_PIN_ENB)); // ENA & ENB = 0
    //PORTK = 0x00;
}

void setMotorSpeed(int rpm)
{
    gTachometerValue = readTachometer();
    gCurrentRPM = tachometer2rpm(gTachometerValue, (rpm<0)? MOTOR_BACKWARD : MOTOR_FORWARD);

    if(rpm == 0){ // break
        writeMotorPWM(0);
    }else{
        writeMotorPWM(motorPI(rpm - gCurrentRPM));
    }

}

inline int getMotorRPM(void)
{
    return gCurrentRPM;
}

void writeMotorPWM(int pwm)
{
    static int lastPWM = 0;

    if(pwm == 0){           //secure that 0 pwm will always stop motor in all cases bacause min pwm can be over 0
        PORTK &= 0xfc;      //break: 00
        OCR4A = 0;          //set pwm sycle to zero
        lastPWM = 0;
        return;
    }

    //limit acceleration
    if(abs(pwm - lastPWM) > MOTOR_ACC_MAX){
        pwm = lastPWM + (((pwm - lastPWM) < 0) ? -1*MOTOR_ACC_MAX : MOTOR_ACC_MAX);
    }
    //limit control values
    if(pwm > MOTOR_CONTROL_MAX){
        pwm = MOTOR_CONTROL_MAX;
    }else if(pwm < MOTOR_CONTROL_MIN){
        pwm = MOTOR_CONTROL_MIN;
    }

    if(lastPWM != pwm){
        if(pwm > 0)
        {
            PORTK &= 0xfc;       //Clockwise, clear bits
            PORTK |= (1 << MOTOR_PIN_INA);
            OCR4A = pwm;
        }else{
            PORTK &= 0xfc;       //Counter-clockwise (CCW)
            PORTK |= (1 << MOTOR_PIN_INB);
            OCR4A = abs(pwm);   //set pwm value positive
        }
        lastPWM = pwm;
    }
}

//val tacho values from last 10 readings ~1sec
int tachometer2rpm(uint16_t val, uint8_t dir){
    val = val * 1000/LOOP_TIME_MS/TACHOMETER_BUFFER_SIZE*60/TICKS_PER_ROTATION;  // 100ms 2 tic per rotation. 1 tic -> 10 tic per sec 5 rotation per sec 300rpm
    if(dir == MOTOR_FORWARD){
        return (int)val;
    }else{
        return -1*(int)val;
    }
}

int motorPI(int error_in_speed){
    error_in_speed /= MOTOR_SCALE_RPM;
	int control = MOTOR_P * error_in_speed;
	static int motor_I_sum = 0;

    motor_I_sum += MOTOR_I * error_in_speed;

    //integer wind up
    if(abs(motor_I_sum) > MOTOR_I_VALUE_MAX){
        motor_I_sum = (motor_I_sum < 0) ? -1*MOTOR_I_VALUE_MAX : MOTOR_I_VALUE_MAX;
    }
    control += motor_I_sum;

    return control;
}

//this function gives sum of 10 last readings from tacho meter
uint16_t readTachometer(void)
{
    static uint8_t tachoBufferCursor = 0;
    static uint8_t buffer[TACHOMETER_BUFFER_SIZE] = {0};

	buffer[tachoBufferCursor] = TCNT5;
    tachoBufferCursor = (tachoBufferCursor + 1) % TACHOMETER_BUFFER_SIZE;
	TCNT5 = 0;
    uint16_t temp = 0;
    for(uint8_t i = 0; i < TACHOMETER_BUFFER_SIZE; i++){
        temp += buffer[i];
    }
	return temp;
}
