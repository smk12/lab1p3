/*
* File:   leds.c
* Author: 
*
* Created on December 27, 2014, 1:31 PM
*/

#include <xc.h>

#define INPUT 1
#define OUTPUT 0

void initLEDs(){
    //TODO: Initialize the appropriate pins to work with the LEDs
    TRISGbits.TRISG12 = OUTPUT;
    TRISGbits.TRISG14 = OUTPUT;
    
}
