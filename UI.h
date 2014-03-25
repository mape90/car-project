#ifndef UI_hh
#define UI_hh

#include "includes.h"

void handleButtonPress(void);
unsigned char USART_Receive(void);
void USART_LCD_Init(unsigned int ubrr);
void LCD_update();
void LCD_state(uint8_t state);
void LCD_loops(uint8_t loop);
void LCD_clear(void);

void LCD_Write(void);

void LCD_ClearErrors(void);

void LCD_WriteError(uint8_t errNo);

void LCD_setTimer(void);


#endif
