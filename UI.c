#include "includes.h"

/* External global variables */
extern uint8_t gState;
extern bool gUICanUpdate;
extern int8_t gPrevPositionError;
extern int gCurrentRPM;
extern uint8_t gLCDErrorFlags;


void LCD_update()
{
    if(gUICanUpdate){
        LCD_clear();
        LCD_state(gState);
        LCD_error(gPrevPositionError);
        LCD_writeInt(gCurrentRPM, 10);
        LCD_setTimer();
        gUICanUpdate = false;
    }else{
        //Do nothing
    }
}

/* not in use
inline void reportError(uint8_t errNo)
{
    if(!(gLCDErrorFlags & (0x01 << errNo))){
        //new error causes instant LCD update
        LCD_WriteErrorFlags(errNo);
        timer_zero_value(TIMER_2);
    }
}
*/
/* Not in use
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
*/
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

void LCD_clearRow(uint8_t row)
{
    char emptyLine[] = "                    ";
    LCD_writeString(emptyLine, row);
}

void LCD_writeString(char* str, uint8_t row)
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
	char state_char[15];
	switch (gState)
	{
		case STATE_WAIT:
            strncpy(state_char, "State: WAIT   \0", 15);
			break;
		case STATE_RUNNING:
            strncpy(state_char, "State: RUNNING\0", 15);

			break;
		case STATE_FINDING_ROAD:
            strncpy(state_char, "State: FINDING_ROAD\0", 20);

			break;
		case STATE_ROAD_NOT_FOUND:
            strncpy(state_char, "State:ROAD_NOT_FOUND\0", 21);

			break;
        default:
            strncpy(state_char, "Unknown State!\0", 15);
	}

	LCD_writeString(state_char,ROW_1);

}

void LCD_error(char error)
{
	char err[14];
	if (error == GOAL_POINT)
	{
		strncpy(err, "GOAL POINT\0", 10);
	}
	else if (error == CONTROL_NO_REF_POINT)
	{
		strncpy(err,"NO REF POINT\0", 12);
	}
	else
	{
		itoa((int)error, err, 10);
	}
	char str[25] = "Error: ";
	strncat(str, err,strlen(err));
	LCD_writeString(str,ROW_2);
}

void LCD_speed(char* speed)
{
	char str[25] = "Speed: ";
	strcat(str,speed);
	LCD_writeString(str, ROW_3);
}

void LCD_writeInt(int num, uint8_t row)
{
	char temp[10];
	itoa(num, temp,10);

	LCD_writeString(temp,row);
}

void LCD_clear(void)
{
	USART_Transmit(CLEAR);
	while (RECEIVED != USART_Receive());
}
inline void LCD_clearErrorFlags(void)
{
    gLCDErrorFlags = 0;
}

inline void LCD_writeErrorFlags(uint8_t errNo)
{
    gLCDErrorFlags |= (0x01 << errNo);
}

inline void LCD_setTimer(void)
{
    timer_enable(TIMER_2, LCD_REFRESH_RATE_MS);
}
