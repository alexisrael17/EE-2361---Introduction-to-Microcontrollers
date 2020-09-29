#include "xc.h"
#include "lemac_lab4_button.h"
#include "lema_lab4_servo.h"
#include <p24Fxxxx.h>
#include <p24FJ64GA002.h>
#include "lema_lab4_delayHeader.h"

//#define timeCALCULATED 125000

volatile long int overFW=0;

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

