#include <p24Fxxxx.h>
#include <p24FJ64GA002.h>
#include "xc.h"
#include "lema_lab4_delayHeader.h"


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

void initServo(void);
void delay(int delay_in_ms);
void setServo(int Val);
volatile long int overFW=0;
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
void initPushButton(void);
void doubleDETECTOR();

void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void)
{
    _T2IF = 0;        
    overFW++;
}
void initPushButton(void)
{

    CLKDIVbits.RCDIV =0;                    //set the clock frequency to 16 MHz
    AD1PCFG = 0x9fff;                       //sets all pins to digital
    TRISB &= 0xFFBF;                        //sets RB8 as a digital input port
    CNPU2bits.CN22PUE = 1;//pull-up resistors PIN17
    LATBbits.LATB8 = 1;  //PIN17
    
    T2CON = 0x0030;                    // TCKPS 1:256 
    TMR2 = 0;                               //start counting up from 0
    PR2 = 65535; //  about 1.04856 sec delay
    IPC1bits.T2IP = 0x01;
    _T2IF = 0;
    _T2IE = 1; 
 
    //T2CON = 0x8000;
    T2CONbits.TON = 1;                      //start with the timer off 
    PORTBbits.RB8 &= 1;
    
}
    
volatile  long int timeDifference;
volatile unsigned long int firstPress, secondPress, curEdge, curPeriod, prevEdge;

void doubleDETECTOR()
{    
    int clickCount=0;
    //long int timeCALCULATED = 150000;
    while(1)
    {
      while(_RB8 == 0)
      {
        delay(2); 
        if(_RB8 == 1)
        {  
          clickCount++; 
          if (clickCount==1){
          firstPress = TMR2;}
          if (clickCount==2){
          secondPress = TMR2;}
        }
      } 
  
      if((clickCount== 2))
      {
        timeDifference = secondPress - firstPress + 65536*overFW; 
        if((timeDifference >100000)  &&(overFW>2))
        { 
          overFW = 0;
          clickCount= 0;
        }
        else
        {
          setServo(1000);
          delay(1000);         
          setServo(3000);
          clickCount = 0;
          overFW = 0;
  
        }
     }
    }
}

void delay(int delay_in_ms) // as the TA told as to use assembly for the delay
{
    while(delay_in_ms>0)
    {
      lem_wait_1ms(); // this funtion works with asembly.s
      delay_in_ms--; // this funtion works with asembly.s
    }//end while// 
}//end delay
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
int main(void) 
{ 
   
   initServo();    
   initPushButton();
   doubleDETECTOR();
   
}
