#include "lema_lab4_servo.h"
#include <p24Fxxxx.h>
#include <p24FJ64GA002.h>
#include "xc.h"

   
void initServo(void){  
    
    __builtin_write_OSCCONL(OSCCON & 0xbf);    // unlock PPS            
   // RPINR7bits.IC1R = 8; // Use Pin RP8 = "8", for Input Capture 1 (Table 10-2)
    RPOR3bits.RP6R = 18; // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40);   // lock PPS              // re-lock PPS
    
    CLKDIVbits.RCDIV =0;                    //set the clock frequency to 16 MHz
    AD1PCFG = 0x9fff;                       //sets all pins to digital
    TRISB &= 0xFFBF;                        //sets RB6 as a digital output port
   ///////////////////////////////////////////////
    T3CONbits.TON = 0;                      //start with the timer off
    T3CON = 0x0010;                       // TCKPS 1:8 (sets prescaler)
    TMR3 = 0;                               //start counting up from 0
    _T3IF = 0;
    PR3 = 40000;                            // PR = 40000 to achieve 20 ms delay
   
    OC1R = 3000; //start at the 1.5 ms position (0 degrees +/-) ***
    OC1RS=3000;
    OC1CONbits.OCTSEL = 1;                  // use Timer 3
    OC1CONbits.OCM = 0b110;                 // set PWM mode, disable fault pin (page 11 of lab manual)
    T3CONbits.TON = 1;                          //starts the timer   
    
}


void setServo(int Val){
    OC1RS= Val;  
}