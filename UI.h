#ifndef UI_HH
#define UI_HH

void reportError(uint8_t errNo);
void handleButtonPress(void);

void USART_Transmit( unsigned char data);
unsigned char USART_Receive(void);
void USART_LCD_Init(unsigned int ubrr);
void LCD_state(uint8_t state);
void LCD_loops(uint8_t loop);
void LCD_clear(void);

#endif
