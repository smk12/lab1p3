// ******************************************************************************************* //
//
// File:         lab1p2.c
// Date:         12-30-2014
// Authors:      Garrett Vanhoy
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "leds.h"
#include "timer.h"
#include "config.h"
#include "switch.h"
#include "interrupt.h"


// ******************************************************************************************* //

#define ON 0
#define OFF 1
#define PRESS 0
#define RELEASE 1
#define led1 LATGbits.LATG12
#define led2 LATGbits.LATG14

typedef enum stateTypeEnum{
    RUN, STOP, debouncePress, debounceRelease
} stateType;

volatile stateType state;
volatile stateType lastState;
volatile int temp = 0;      volatile int temp2 = 0;
volatile int min10 = 0;     volatile int min01 = 0;
volatile int sec10 = 0;     volatile int sec01 = 0;
volatile int ms10 = 0;      volatile int ms01 = 0;

volatile char sc = ':';

/* Please note that the configuration file has changed from lab 0.
 * the oscillator is now of a different frequency.
 */
int main(void)
{
    SYSTEMConfigPerformance(40000000);
    enableInterrupts();
    initLEDs();
    initLCD();
    initTMR1();
    initSW2();
    state = RUN;
    
    while(1)
    {
        switch(state)
        {
            case RUN:
                led1 = ON;
                led2 = OFF;
                clearLCD();
                printStringLCD("Running:");
                moveCursorLCD(1,0);
                printCharLCD((char)min10);printCharLCD((char)min01);printCharLCD(sc);
                printCharLCD((char)sec10);printCharLCD((char)sec01);printCharLCD(sc);
                printCharLCD((char)ms10);printCharLCD((char)ms01);
                lastState = RUN;
                T1CONbits.ON = 1;
                break;
                
            case STOP:
                T1CONbits.ON = 0;
                led1 = OFF;
                led2 = ON;
                clearLCD();
                printStringLCD("Stopped:");
                moveCursorLCD(1,0);
                printCharLCD((char)min10);printCharLCD((char)min01);printCharLCD(sc);
                printCharLCD((char)sec10);printCharLCD((char)sec01);printCharLCD(sc);
                printCharLCD((char)ms10);printCharLCD((char)ms01);
                lastState = STOP;
                break;
                
            case debouncePress:
                delayMs(5);
                if(lastState==RUN)
                    state = STOP;
                else if(lastState==STOP)
                    state = RUN;
                else
                    state = RUN;
                break;
                
            case debounceRelease:
                delayMs(5);
                state = lastState;
                break;
        }
    }
    
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void)
{
    //TODO: Implement the interrupt to capture the press of the button
    
    temp = PORTGbits.RG13;
    temp2 = PORTDbits.RD6;
    IFS1bits.CNGIF = 0;
    if(temp2==PRESS & lastState==STOP)
    {
        min10 = 0;min01 = 0;sec10 = 0;sec01 = 0;ms10 = 0;ms01 = 0; TMR1 = 0;
    }
    else if(temp==PRESS)
        state = debouncePress;
    else if(temp==RELEASE)
        state = debounceRelease;
    else
        state = lastState;
    temp = -1;
    
    /*
    temp = PORTDbits.RD6;
    IFS1bits.CNDIF = 0;
     */
}

void __ISR(_TIMER_1_VECTOR, IPL3SRS) _T1Interrupt()
{
    IFS0bits.T1IF = 0;
    ms01++;
    if(ms01>=10)
    {
        ms10++;
        ms01-10;
        if(ms10>=10)
        {
            sec01++;
            ms10-10;
            if(sec01>=10)
            {
                sec10++;
                sec01-10;
                if(sec10>=6)
                {
                    min01++;
                    sec10-6;
                    if(min01>=10)
                    {
                        min10++;
                        min01-10;
                        if(min10>=6)
                        {
                            min10 = 0;min01 = 0;sec10 = 0;sec01 = 0;ms10 = 0;ms01 = 0; TMR1 = 0;
                        }
                    }
                }
            }
        }
    }
}
