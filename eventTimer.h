
#ifndef EVENT_TIMER_HH
#define EVENT_TIMER_HH

void initEventTimer(void);
void timer_setValue(uint8_t timer, uint16_t value);
uint16_t timer_getValue(uint8_t timer);
void timer_setCurrentValue(uint8_t timer, uint16_t value);
uint16_t timer_getCurrentValue(uint8_t timer);
#endif
