#include "xc.h"
#include "lema_lab3_header.h"
#include "lema_lab3_displayHeader.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
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
        
void setup(void)
{
    CLKDIVbits.RCDIV = 0; // 8MHz//gives the builtin 8mhz oscillator, not divided by a postscaler, and
    //finally divided by two = 4MHz. 
    init7seg();
  
}

void delay(int delay_in_ms) // as the TA told as to use assembly for the delay
{
    while(delay_in_ms>0)
    {
      lem_wait_1ms(); // this funtion works with lema_lab3_asmLib.s
      delay_in_ms--; // this funtion works with lema_lab3_asmLib.s
    }//end while// 
}//end delay
int main(void) 
{
    setup();//for postscaler, and setup LATB LATA
    
    
    //<<PRELAB OUTPUT>*******************************
    while(1)    //I use this while loop to check the dual 7 segment display
      {
        lemaTest7seg();
       }//*/    //end while******************************
    
 
     
    return 0;
}//end int main(void)

