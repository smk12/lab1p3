/*
 * File:   switch.c
 * Author: Garrett
 *
 * Created on September 19, 2015, 10:46 AM
 */

#include <xc.h>

#define INPUT 1
#define OUTPUT 0

void initSW2(){
    //TODO: Initialize the appropriate pins to work with the external switch using a
    //change notification interrupt.
    
    TRISGbits.TRISG13 = INPUT;
    CNCONGbits.ON = 1;      //CN for G 
    CNENGbits.CNIEG13 = 1;  //CN on pin
    IEC1bits.CNGIE = 1;     //overall CN
    CNPUGbits.CNPUG13 = 1;  //PUR
    IPC8bits.CNIP = 7;
    IPC8bits.CNIS = 2;
    IFS1bits.CNGIF = 0;
     
    
    TRISDbits.TRISD6 = INPUT;
    CNCONDbits.ON = 1;
    CNENDbits.CNIED6 = 1;
    IEC1bits.CNDIE = 1;
    CNPUDbits.CNPUD6 = 1;
    IPC8bits.CNIP = 7;
    IPC8bits.CNIS = 2;
    IFS1bits.CNDIF = 0;
    
}
