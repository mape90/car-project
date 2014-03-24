#include "includes.h"
//--------------TIMER-----------------//

/* Global variables */
extern volatile bool gLoopTimeNotElapsed;

static uint16_t timer_current_value[TIMER_MAX_COUNT];
static uint16_t timer_set_value[TIMER_MAX_COUNT];

inline void initEventTimer(void)
{
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
