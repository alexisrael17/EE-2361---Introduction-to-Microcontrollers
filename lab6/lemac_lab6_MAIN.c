/*
 * File:   lemac_lab6_MAIN.c
 * Author: Alex
 *
 * Created on November 13, 2019, 2:44 AM
 */


#include "xc.h"
#include "lemac_lab6_lcd.h"
#include "lemac_lab6_Buffer_H.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include"stdlib.h"

#pragma config ICS = PGx1 // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF // JTAG Port Enable (JTAG port is disabled)

// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME // Clock Switching and Monitor (Clock switching is enabled,
                                // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


unsigned char displayFlag = 0;

void delay(int delay_in_ms) // delay in miliseconds 
{
    while(delay_in_ms>0)
    {
      lem_wait_1ms(); // this funtion works with assembly file and header 
      delay_in_ms--; // counter/ decrement 
    }//end while(delay_in_ms>0) 
}//end delay



 #define LOOP_COUNT 5

volatile int timerLoop = LOOP_COUNT; // Start by displaying voltage

//volatile int value=0;

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    // When ticks >2 than move the servo to neutral
    char outString[20];
    double result = 0.0;
    
    if(timerLoop++ > LOOP_COUNT)
    {
        timerLoop = 0;
        // We have looped enough, display the time
        result = (3.3/1024)*getAvg();
        sprintf(outString, "%6.4f V" , result);
        lcd_printStr(outString);
    }
    // TMR2 = 0;
    IFS0bits.T2IF = 0;   

}

void __attribute__((__interrupt__,  __auto_psv__)) _ADC1Interrupt(void) {
    putVal(ADC1BUF0);
    AD1CON1bits.SAMP = 1;   // start sampling
    _AD1IF = 0; // clear flag

}

void setup(void) {
    CLKDIVbits.RCDIV = 0;

    AD1PCFG = 0xfffe; // AN0 is the only analog pin we are using
    lcd_init();
    initBuffer();

    // Setup on A/D
    AD1CON1 = 0;
    AD1CON2 = 0;
    AD1CON3 = 0;
    AD1CHS = 0;
    

    //AD1CON1bits.ASAM = 1; // auto-sampling?
    AD1CON1bits.SSRC = 0b111; // auto-conversion

    AD1CON3bits.SAMC = 10; // auto-sample time = X * TAD (what is X?)
    AD1CON3bits.ADCS = 2; // clock = Y * Tcy (what is Y?)
    
    // what is the combined T_SAMP and T_CONV as a result of the two lines above?

    // Configure A/D interrupt
    _AD1IF = 0; // clear flag
    _AD1IE = 1; // enable interrupt

    AD1CON1bits.ADON = 1;
    
   // Configure Timer 2 for display every 500 milliseconds
    // 1/((62.5*(10^(-9)))*256) = 62500 using 256 prescaler
    T2CON = 0x30; // prescaler set to 1:256
    PR2 = 625; // 1 second
    TMR2 = 0; // Clear the timer count

    IFS0bits.T2IF = 0;   
    IEC0bits.T2IE = 1; //Enable Timer3 interrupts
    T2CONbits.TON = 1; // Start the timer

}

int main(void) {
    setup();
    
//    AD1CON1bits.SAMP = 1;   // start sampling

    AD1CON1bits.SAMP = 1;   // start sampling
    while (1);
    // {
        // AD1CON1bits.SAMP = 1;   // start sampling
        // while(!AD1CON1bits.DONE);   // wait until conversion is done
        // val = ADC1BUF0;
    // }
}

 
 