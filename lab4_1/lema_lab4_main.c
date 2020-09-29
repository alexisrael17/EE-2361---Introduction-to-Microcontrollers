#include <p24Fxxxx.h>
#include <p24FJ64GA002.h>
#include "xc.h"

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config SOSCSEL = SOSC           // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

unsigned char pressed = 0;

void setup(void){
    
        // add your configuration commands below
    CLKDIVbits.RCDIV =0;                    //set the clock frequency to 16 MHz
    AD1PCFG = 0x9fff;                       //sets all pins to digital
    PORTB = 0xffff;
    TRISB = 0xff7f; // bit 15 is an output
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8; // Use Pin RP8 = "8", for Input Capture 1 (Table 10-2)
    RPOR3bits.RP6R = 18; // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    
    initButton();
    initServo();
}
//////////////////////////////////////////////////////////////////
void initServo(void)
{
    T3CONbits.TON = 0;
    T3CON = 0x10; // 0x00; //8010;
    PR3 = 40000;
    TMR3 = 0x00; //Clear contents of the timer register
    IPC2bits.T3IP = 5; // set TNR3 interrupt priority
    IFS0bits.T3IF = 0;
    IEC0bits.T3IE = 1; //Enable Timer3 interrupts

    // Timer 3 setup should happen before this line
    OC1CON = 0; // turn off OC1 for now
    OC1R = 3000; // servo start position. We won?t touch OC1R again
    OC1RS = 3000; // We will only change this once PWM is turned on
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
    T3CONbits.TON = 1;

}
int getServo()
{
    return OC1RS;
}
void setServo(int Val)
{
    if(Val >3000)
        OC1RS = 3000;
    else if(Val < 1000)
        OC1RS = 1000;
    else
        OC1RS = Val;
}
///////////////////////////////////////////////////////////////////////
void initButton(void){
    // Timer 2 needs to be ~1 second per cycle
    // 1/((62.5*(10^(-9)))*256) = 62500 using 256 prescaler
    T2CON = 0x30; // 0x10;
    PR2 = 62500;
    T2CONbits.TON = 1;
    TMR2 = 0;
    PORTBbits.RB8 &= 1;

    // resetBuffer();
    IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag
    IEC0bits.IC1IE = 1; // Enable IC1 interrupts
    IEC0bits.T2IE = 1; //Enable Timer2 interrupts

    IPC0bits.IC1IP = 1; // Set module interrupt priority as 1
    IC1CON = 0;
    IC1CONbits.ICTMR = 1; // timer 2
    IC1CONbits.ICM = 0x2;   // capture on every edge
    
}
/////////////////////////////////////////////////
//#define clickSize 15625
// #define clickSize 30000

volatile unsigned char front = 0;
volatile unsigned char back = 0; // this is not moved until front loops the first time
volatile unsigned char loopStarted = 0;
volatile unsigned int cBuffer[2];
volatile int timerOffset = 0; // if we need more thant 32k, I am doing it wrong


void resetBuffer(void)
{
    int counter = 0;
    for(counter=0;counter<2; counter++)
    {
        cBuffer[counter] = 0xFFFF; // max value
    }
    front = 0;
}

// Not using a back pointer, as we can walk the buffer from front

void pushClick(unsigned int newTime)
{
    int breakhere = 1;
    if((newTime < 15625) && (newTime>0))
        breakhere = 1;
    
    cBuffer[front++] = newTime;
        
    if(front>=2)
    {
        front=0;
        back=1; // Back must be 1 past front when it loops always
        loopStarted = 1; // easier this way
    }
    else{
        // Handle back accordingly
        if(loopStarted==1)
            if(++back >= 2)
                back = 0;
    }
}
unsigned char clickCount()
{
    // every time a zero is encountered we reset
    // To do this accurately, would need to start buffer size back from front
    // Or even have a back position
    // Going to use a back buffer pointer...
    
    unsigned char retVal = 1; // start with one click
    unsigned char longCount = 0;
    unsigned int counter = 0;
    unsigned int walker = front;
    if(cBuffer[0] !=0xFFFF)
        counter = 0; // no idea what I was trying to do here...

    while(counter++<2){
        if((cBuffer[walker] < 15625) && (cBuffer[walker] > 0))
            retVal++;
        if(++walker>=2)
            walker=1;
    }
    if(retVal > 1)
        counter = 0;
    return retVal;
}

unsigned char isButtonPressed(){
    return (!IFS0bits.IC1IF);
}

unsigned char isButtonTimeOut(){
    return (IFS0bits.T2IF==1);
}
//--------------------------------------------------------------
#define HIGHWAVE 3000
#define LOWWAVE 1000


int main(void) {
    setup();    
    while (1);
    return 0; // never reached (we hope)
}
void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void)
{
    int clicks = 0;
    // Time-out activity no longer works in the new setup
    // Now we just debounce
    // Debounce event
    // Get the current total count up front, reduce calls
    unsigned long currentCount = (((unsigned long)timerOffset)*((unsigned long)PR2)) + TMR2;
    
    if(currentCount < 80){
        // Ticks do not reset, we want the first edge
        // just reset the status flag
        // This range may need to be recalculated as the timer was changed
        // resetTicks();
        IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag
        return;
    }
    
    pushClick(currentCount);
    
    clicks = clickCount();
    // Buffer version
    if(clicks > 1)
    {
        // We have a double click
        if(getServo() != 3000)
            setServo(3000);
    }
    if(clicks > 2){
        // Triple click
    }
    else{
        // One click, set low
        if(getServo() != 3000)
            setServo(1000);
    }
        
    // Reset Timer2
    TMR2 = 0;
    IFS0bits.T2IF = 0;
    timerOffset = 0;
    IFS0bits.IC1IF = 0; // Clear the IC1 interrupt status flag
}
void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void)
{
    // When ticks >2 than move the servo to neutral   
    TMR2 = 0;
    IFS0bits.T2IF = 0;   
    ++timerOffset;
    if(timerOffset==2)
        if(getServo() != 2000)
            setServo(2000);
}

// Setup timer interrupt
void __attribute__((__interrupt__,__auto_psv__)) _T3Interrupt(void)
{
    // TODO: Put interrupt code here
    // Clear timer 3 and reset it
    TMR3 = 0;
    IFS0bits.T3IF = 0;   

}