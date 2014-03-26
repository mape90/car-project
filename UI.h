#ifndef UI_hh
#define UI_hh

#include "includes.h"

void LCD_update();
void handleButtonPress(void);

void reportError(uint8_t errNo);

void USART_Transmit( unsigned char data);
unsigned char USART_Receive(void);
void USART_LCD_Init(unsigned int ubrr);

void LCD_Write_String(char* str, uint8_t row);
void LCD_state(uint8_t state);
void LCD_error(char error);
void LCD_speed(char* speed);
void LCD_clear(void);

void LCD_Write(void);
void LCD_ClearErrorFlags(void);
void LCD_WriteErrorFlags(uint8_t errNo);
void LCD_setTimer(void);


#endif
