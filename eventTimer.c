#include "includes.h"
//--------------TIMER-----------------//

/* Global variables */
static uint16_t timer_current_value[TIMER_MAX_COUNT];
static uint8_t timer_flags = 0;

/* External Global Variables */
extern volatile bool gLoopTimeNotElapsed;
extern bool gUICanUpdate;
extern bool gFindRoadTimerElapsed;

inline void initEventTimer(void)
{
    TCCR0B |= ((1 << CS01)|(1 << CS00)); // 64
    TCNT0 = 0;
    TIMSK0 |= _BV(TOIE0);

    timer_enable(TIMER_1, LOOP_TIME_MS);
}

inline void timer_enable(uint8_t timer, uint16_t val)
{
    timer_current_value[timer] = val;
    timer_flags |= (0x01 << timer);
}
inline void timer_disable(uint8_t timer)
{
    timer_flags &= ~(0x01 << timer);
}
inline bool timer_enabled(uint8_t timer)
{
    return timer_flags & (0x01 << timer);
}
inline bool timer_ended(uint8_t timer)
{
    return timer_current_value[timer] == 0;
}
inline void timer_update(uint8_t timer)
{
    (timer_current_value[timer])--;
}

inline void timer_zero_value(uint8_t timer)
{
  timer_current_value[timer] = 0;
}

inline void disable_wait(void)
{
    gLoopTimeNotElapsed = false;
}

inline void disableGuiUpdateWait(void)
{
    gUICanUpdate = true;
}

inline void changeToRoadNotFoundState(void)
{
    gFindRoadTimerElapsed = true;
}

//-----------------------------------//
