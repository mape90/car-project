#include "includes.h"

/* External global variables */
extern uint8_t gState;
extern bool gUICanUpdate;
extern char gLastError;
extern int gCurrentRPM;
extern uint8_t gLCDErrorFlags;


void LCD_update()
{
    if(gUICanUpdate){
        LCD_clear();
        LCD_state(gState);
        LCD_error(&gLastError);
        { //TODO: check if this can be done some other way
            char temp_buffer[10];
            itoa (gCurrentRPM, temp_buffer, 10);
            LCD_speed(temp_buffer);
        }

        LCD_setTimer();
    }else{
        //Do nothing
    }
}

inline void reportError(uint8_t errNo)
{
    if(!(gLCDErrorFlags & (0x01 << errNo))){
        //new error causes instant LCD update
        LCD_WriteErrorFlags(errNo);
        timer_zero_value(TIMER_2);
    }
}

void handleButtonPress(void)
{
	switch(gState)
	{
		case STATE_WAIT:
			setNewState(STATE_RUNNING);
			break;
		case STATE_RUNNING:
		case STATE_ROAD_NOT_FOUND:
			setNewState(STATE_WAIT);
			break;
		default:
			//LCD info?
			break;
    }
}

void USART_Transmit( unsigned char data)
{
	// Wait for empty transmit buffer
	while ( !( UCSR1A & (1<<UDRE1)) )
		;
	// Put data into buffer, sends the data
	UDR1 = data;
}

unsigned char USART_Receive(void){
	// Wait for data to be received
	while ( !(UCSR1A & (1<<RXC1)) )
		;
	// Get and return received data from buffer
	return UDR1;
}
void USART_LCD_Init(unsigned int ubrr){
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;

	UCSR1B = (1<<RXEN1)|(1<<TXEN1);
	UCSR1C = (1<<UCSZ11) | (1<<UCSZ10);

	// LCD init
	_delay_ms(510);
	USART_Transmit(AUTO_BAUD);
	while (RECEIVED != USART_Receive());
}

void LCD_Write_String(char* str, uint8_t row)
{
	USART_Transmit(DRAW_STRING);
	USART_Transmit(COLUMN_0);
	USART_Transmit(row);
	USART_Transmit(FONT);
	USART_Transmit(WHITE);
	USART_Transmit(WHITE);
	{
		char* c = str;
		do {
			USART_Transmit(*c);
		} while (*c++ != '\0');
	}
	USART_Transmit('\0');
	while (RECEIVED != USART_Receive());
}

void LCD_state(uint8_t gState)
{
	char* state_char;
	switch (gState)
	{
		case STATE_WAIT:
			state_char = "State: WAIT";
			break;
		case STATE_RUNNING:
			state_char = "State: RUNNING";
			break;
		case STATE_FINDING_ROAD:
			state_char = "State: FINDING_ROAD";
			break;
		case STATE_ROAD_NOT_FOUND:
			state_char = "State:ROAD_NOT_FOUND";
			break;
        default:
			state_char = "Unknown State!";
	}
	LCD_Write_String(state_char,ROW_1);

}

void LCD_error(char* error)
{
	char str[25] = "Error: ";
	strcat(str,error);
	LCD_Write_String(str,ROW_2);
}

void LCD_speed(char* speed)
{
	char str[25] = "Speed: ";
	strcat(str,speed);
	LCD_Write_String(str,ROW_3);
}

void LCD_clear(void)
{
	USART_Transmit(CLEAR);
	while (RECEIVED != USART_Receive());
}
inline void LCDClearErrorFlags(void)
{
    gLCDErrorFlags = 0;
}

inline void LCD_WriteErrorFlags(uint8_t errNo)
{
    gLCDErrorFlags |= (0x01 << errNo);
}

inline void LCD_setTimer(void)
{
    timer_enable(TIMER_2, LCD_REFRESH_RATE_MS);
}
