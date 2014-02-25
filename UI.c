#include "includes.h"

/* External global variables */
extern uint8_t gState;

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

void updateLcd(void){
	//show state
    //show error
	//show direction and speed
	return;
}

