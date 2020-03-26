#include "msp.h"

//2.7 2.6 11000000

void init_WheelMotors(void){

    P2->SEL0 &= ~0xC0;
    P2->SEL1 &= ~0xC0;      // configure P2.7-P2.6 as GPIO
    P2->DIR  |=  0xC0;      // make P2.7-P2.6 an output
    P2->OUT  &= ~0xC0;      // all outputs LOW

}

void goForward(void){

    P2->OUT &= ~0xC0;
    P2->OUT |= 0x80;        // set P2.7 HIGH, P2.6 LOW

}

void goBackward(void){

    P2->OUT &= ~0xC0;
    P2->OUT |= 0x40;        // set P2.7 LOW, P2.6 HIGH

}

void stopWheelMotors(void){

    P2->OUT &= ~0xC0;

}


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	init_WheelMotors();

	while(1){

        goForward();

        goBackward();

        stopWheelMotors();

	}

}
