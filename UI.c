#include "includes.h"

/* External global variables */
extern uint8_t gState;

void LCD_update()
{
   // Todo: implement this
}

void reportError(uint8_t errNo) {
 // Todo: Implement this
}

void handleButtonPress(void){
	switch(gState){
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

void USART_Transmit( unsigned char data){
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
	unsigned char data_rec = RECEIVED;
	while (data_rec != USART_Receive())
		;
}



void LCD_state(uint8_t state){
	char* state_char;
	switch (state){
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
	}

	USART_Transmit(DRAW_STRING);
	USART_Transmit(COLUMN_0);
	USART_Transmit(ROW_1);
	USART_Transmit(FONT);
	USART_Transmit(WHITE);
	USART_Transmit(WHITE);
	{
		char* c = state_char;
		do {
			USART_Transmit(*c);
		} while (*c++ != '\0');
	}
	USART_Transmit('\0');
	while (RECEIVED != USART_Receive());
}

void LCD_loops(uint8_t loop){
	char loop_char[] = "Loop: ";
	char temp[10];
	itoa(loop, temp,10);
	strcat(loop_char,temp);

	USART_Transmit(DRAW_STRING);
	USART_Transmit(COLUMN_0);
	USART_Transmit(ROW_2);
	USART_Transmit(FONT);
	USART_Transmit(WHITE);
	USART_Transmit(WHITE);
	{
		char* c = loop_char;
		do {
			USART_Transmit(*c);
		} while (*c++ != '\0');
	}
	USART_Transmit('\0');
	while (RECEIVED != USART_Receive());
}

void LCD_clear(void){
	USART_Transmit(CLEAR);
	while (RECEIVED != USART_Receive());
}
