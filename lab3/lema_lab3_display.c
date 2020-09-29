/*
 * File:   lema_lab3_display.c
 * Author: Alex
 *
 * Created on July 6, 2019, 12:13 AM
 */


#include "xc.h"
#include "lema_lab3_displayHeader.h"

void delay(int delay_in_ms);
void lemaTest7seg(void); 
void init7seg(void);
void showChar7seg(char myChar, int myDigit);

void lemaTest7seg(void) 
{   int counter5=0,a=0,b=1;
    while(counter5<16)
    {
     if(a==0)
     {  showChar7seg(a, LSB);
        delay(450);
     }
     showChar7seg(a, MSB); 
     delay(450);
     showChar7seg(b, LSB);
     delay(450);
     counter5++,a++,b++;
    }
    lemaTest7seg(); 
 
}//*/

const unsigned int arrar_binary[] = {//I am using this for output and input
    //RB9  TO RB3   
    //RB2 TO RB0
          /*             DUAL 7 DISPLAY
           *                      A                        A 
                          +---+---+---+---+        +---+---+---+---+
                          |               |        |               |
                        F +               + B    F +               + B
                          |               |        |               | 
                          + --+---+---+-- +        + --+---+---+-- +
                          |               |        |               | 
                        E +               + C    E +               + C
                          |               |        |               |     
                          +---+---+---+---+   OO   +---+---+---+---+    OO
                                  D           DP           D            DP
           * 
                               PIN                    PIN               RB
           *                 A=16          |           A=11    |          RB 9
           *                 B=15          |           B=10    |          RB 8           *   
           *                 C=3           |           C=8     |          RB 7
           *                 D=2           |           D=6     |          RB 6
           *                 E=1           |           E=5     |          RB 5 
           *                 F=18          |           F=12    |          RB 4
           *                 G=17          |           G=7     |          RB 3           *   
           *                DP=4           |          DP=9     |          RB 2
           * O >>> LED ON
           * 1 >>> LED OFF
 
           */
    
                0b1100, //0
          0b1001111100, //1
            0b10010100, //2
              0b110100, //3
          0b1001100100, //4
           0b100100100, //5
           0b100000100, //6
             0b1111100, //7
                 0b100, //8
              0b100100, //9
             0b1000100, //A
          0b1100000100, //b  
           0b110001100, //C
          0b1000010100, //d
           0b110000100, //E
           0b111000100, //F
          0b1111111100,   
    //I will add some more for other digits to display later
               
      /* 
                          +---+---+---+---+
                          | 1 | 2 | 3 | A |
                          +---+---+---+---+
                          | 4 | 5 | 6 | b |
                          +---+---+---+---+
                          | 7 | 8 | 9 | C |
                          +---+---+---+---+
                          | F | 0 | D | d |
                          +---+---+---+---+
       */
};


void init7seg(void) 
{
    AD1PCFG = 0xffff; ;         // Initialize  pins as digital
    TRISB &= 0b1111000000000011; // initialize PORTB as output
}


void showChar7seg(char myChar, int myDigit) 
{    
    LATB &= 0b1111000000000011; //turn off all displays
    LATB = LATB | myDigit | arrar_binary[myChar]; 
    /*
     * Example:
      11001000  //LATB
    | 10111000   //myDigit = 
    | 10001000   // arrar_binary[myChar]=

      -------- 
    = 11111000   ///LATB= 
     
     */
}


