#include "includes.h"
//--------------TIMER-----------------//

/* Global variables */
static uint16_t timer_current_value[TIMER_MAX_COUNT];
static uint16_t timer_set_value[TIMER_MAX_COUNT];

/* External Global Variables */
extern volatile bool gLoopTimeNotElapsed;

inline void initEventTimer(void)
{
    TCCR0B |= ((1 << CS01)|(1 << CS00)); // 64
    TCNT0 = 0;
    TIMSK0 |= _BV(TOIE0);

    timer_set_value[TIMER_1] = LOOP_TIME_MS;
}

inline void timer_setValue(uint8_t timer, uint16_t value)
{
    timer_set_value[timer] = value;
}

inline uint16_t timer_getValue(uint8_t timer)
{
    return timer_set_value[timer];
}

inline void timer_setCurrentValue(uint8_t timer, uint16_t value)
{
    timer_current_value[timer] = value;
}

inline uint16_t timer_getCurrentValue(uint8_t timer)
{
    return timer_current_value[timer];
}
inline void disable_wait(void)
{
    gLoopTimeNotElapsed = false;
}
inline void timer_2_function(void)
{

}
inline void timer_3_function(void)
{

}

//-----------------------------------//
