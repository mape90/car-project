#ifndef UI_hh
#define UI_hh

#include "includes.h"

void LCD_update(void);

void            USART_Transmit( unsigned char data);
unsigned char   USART_Receive(void);
void            USART_LCD_Init(unsigned int ubrr);

void            LCD_write(void);
void            LCD_writeInt(int num, uint8_t row);
void            LCD_writeString(char* str, uint8_t row);
void            LCD_state(uint8_t state);
void            LCD_error(char error);
void            LCD_speed(char* speed);

void            LCD_clear(void);
void            LCD_clearRow(uint8_t row);


void            LCD_clearErrorFlags(void);
void            LCD_writeErrorFlags(uint8_t errNo);
void            LCD_setTimer(void);

/* Not in use */
//void handleButtonPress(void);
//void reportError(uint8_t errNo);

#endif
