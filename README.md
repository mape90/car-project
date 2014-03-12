car-project
===========

States
======
1 Wait_state
2 running_stete
3 find_road_state
4 road_not_found_state
 


Motor 
=======
 * PH3 (Timer4)
 * INA - PK0 (ADC8)
 * INB - PK1 (ADC9)
 * ENA - PK2 (INPUT)
 * ENB - PK3 (INPUT)

Servo 
=======
 * Timer1
 * PWM - PB5 (OC1A)

Tachometer 
=======
 * Timer5
 * Signal - PL2 (T5)


LCD
=======
 * RX - PD2 (RXD1)
 * TX - PD3 (TXD1)

Led
=======
 * PC0 - led0 
 * PC1 - led1
 
Button 
=======
 *(HIGH when not pushed)
 * PE5 (INT5)
 


#Done / ToDo List

## General Logic
###Already completed
* Different states for "Find tract", "drive", "wait", "track lost permanetly"
* Set state -function
* handling routine for "find track timer" timeout
* event timer routines to setup future events
* PID
* Loop cycle time synchronizer & constant loop time value

### Setup routines ToDo
* DDR values (port directions)
* 


### Functionality ToDo
* Count loops
* Check if "max loops" reached
* enable timer for finding track (timeout for search)
* Calculate operation logic based on  errors / states
* Calculate turning degree based on PID
* (optional) WatchDog routine

## Front Bumper (reading/reporting position)
### Already completed
* Read bumpers


### Functions ToDo


##Servo (turning)

### Already completed
* Initilize timer clock / prescaler
* Functions


##Motor & Tachometer (speed, direction, feedback)
### Already completed
* Calc speed
* Read tachometer
* Calculate PI

### Functions ToDo
* SetMotorSpeed 
* Tachometer value to speed (scaling)

##User Interface (diagnostics)
### Already completed
- Read button (ISR)
- Set next state based on current state

### Functions ToDO
- Report errors
- Send text to LCD (lib?)
- Update LCD status information
- Set LED states based on "operation mode"

 

