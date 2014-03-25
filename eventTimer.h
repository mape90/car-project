#ifndef EVENT_TIMER_HH
#define EVENT_TIMER_HH

void initEventTimer(void);

void timer_enable(uint8_t timer, uint16_t val);
void timer_disable(uint8_t timer);
bool timer_enabled(uint8_t timer);
bool timer_ended(uint8_t timer);
void timer_update(uint8_t timer);
void timer_zero_value(uint8_t timer);

void disable_wait(void);
void disableGuiUpdateWait(void);
void changeToRoadNotFoundState(void);

#endif
