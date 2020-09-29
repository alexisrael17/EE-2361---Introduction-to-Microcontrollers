/*
 * File:   lemac_lab2b_main_v001.c
 * Author: Alex
 *
 * Created on September 19, 2019, 11:58 AM
 */


#include "xc.h"
#include "lemac_lab2b_Header_v001.h"

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

#define PERIOD 20

void writeColor(int r, int g, int b);
void writeColor1(int r, int g, int b);

void color_cycler_funtion(unsigned char byte);
void delay(int delay_in_ms);
uint32_t wheel(unsigned char wheelPos);
uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu);
unsigned char getR(uint32_t RGBval);
unsigned char getG(uint32_t RGBval);
unsigned char getB(uint32_t RGBval);
void writePacCol(uint32_t packedColor);


void setup() 
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;
    TRISA = 0xfffe; //set pin RA0 to output
    LATA = 0x0000;
    //  LATA = 0xffff;
}
//--------------------------------PRE LAB----------------------------------------------------------
void writeColor1(int r, int g, int b)
{
lem_wait_1ms();
 write_1();
 write_1(); 
 write_1();
 write_1(); 
 
 write_1(); 
 write_1();
 write_1(); 
 write_1(); 
 
 write_0(); 
 write_0(); 
 write_0(); 
 write_0(); 
 
 write_0(); 
 write_0(); 
 write_0(); 
 write_0(); 
 
 write_0();
 write_0(); 
 write_0();
 write_0(); 
 
 write_0(); 
 write_0();
 write_0(); 
 write_0(); 
 
 write_0(); 
 write_0(); 
 write_0(); 
 write_0(); 
 
 write_1(); 
 write_1(); 
 write_1(); 
 write_1(); 
 lem_wait_1ms();
}

void writeColor(int r, int g, int b)
{
int counter1=7;
    lem_wait_1ms();
    //LOOP FOR RED COLOR
    while(counter1>=0)
    {  
        if (r & (1 << counter1))
        { write_1(); }
        else{write_0();}
        counter1--;
    }
    
    counter1=7;
    
       //LOOP FOR GREEN COLOR
    while(counter1>=0)
    {
        if (g & (1 << counter1))
        {write_1(); }
        else{write_0();}
        counter1--;
    }
    
    counter1=7;
       //LOOP FOR BLUE COLOR
     while(counter1>=0)
    {
        if (b & (1 << counter1)) {  write_1(); }
        else {write_0();}
        counter1--;
    }
    counter1=7;

}

//--------------------------------------------------------------------------------------------------------
//assume byteFrameNumber ? {0, ?, 255}
//byteRed = byteFrameNumber;
//byteBlue = 255 - byteFrameNumber;
//PAGE 11 PAR 1

void color_cycler_funtion(unsigned char byte) 
{   //Funtion that does next:
    //Red -> Purple -> Blue -> Purple -> Red and repeats.
    writeColor(byte, 0, 255 - byte);
    
    if(byte==0)//if statement so it will return from blue to purple and to red. 
       { 
        for (byte=0;byte<255;byte++)
                {
                    writeColor(byte, 0, 255 - byte);
                   // writeColor(255 - byte,0,byte);
                    delay(PERIOD);
                }
        }
}
//////////////////////////////////
//===================================================================================================

//////////////page 11//part 4//
void delay(int delay_in_ms) 
{
 
    while(delay_in_ms>0)
    {
      lem_wait_1ms();
      delay_in_ms--;
    }
}
  

//////////////////////Adafruit Wheel() function///////////page12//////////////////////
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.

uint32_t wheel(unsigned char wheelPos) 
{
    wheelPos = 255 - wheelPos;
    
    if(wheelPos < 85) 
    {
        return packColor(255 - wheelPos * 3, 0, wheelPos * 3);
    } 
    
        else if(wheelPos < 170) 
         {
        wheelPos -= 85;
        return packColor(0, wheelPos * 3, 255 - wheelPos * 3);
         } 
    
          else 
             {
                 wheelPos -= 170;
                 return packColor(wheelPos * 3, 255 - wheelPos * 3, 0);
             }
}


//////////////////////////page 13=14///////////////////////
//unsigned char	Of the same size as char, 
//but guaranteed to be unsigned. 
//Contains at least the [0, 255] range.

uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu)
{   ///PAGE 13
   // unsigned long int RGBval = 0;
    return ((long) Red << 16) | ((long) Grn << 8) | ((long) Blu);
    //return RGBval;
}

unsigned char getR(uint32_t RGBval)
{    ///PAGE 13
    //unsigned char Red    = 0;
  //  unsigned long int RGBval = 0x00402060;
    return   (unsigned char) (RGBval >> 16);
    //return Red;
    
}
unsigned char getG(uint32_t RGBval)///PAGE 13
{    ///PAGE 13
    //unsigned char Green  = 0;
   /// unsigned long int RGBval = 0x00402060;
    return  (unsigned char) (RGBval >> 8);
    //return Green;
}

unsigned char getB(uint32_t RGBval)///PAGE 13
{    ///PAGE 13
  //  unsigned char Blue   = 0;
    //unsigned long int RGBval = 0x00402060;
    return   (unsigned char) (RGBval>>0);
//    return Blue;
}

///PAGE 14

void writePacCol(uint32_t packedColor)
{
    writeColor(getR(packedColor), getG(packedColor), getB(packedColor));
}


int main(void) 
{
    setup();
    unsigned char byteFrameNumber=0;
    while(1) 
    {
	//writeColor1(0, 0, 0); 
    //writeColor(0x77, 0x00, 0x00); 
    

        for (byteFrameNumber=255;byteFrameNumber>=0;byteFrameNumber--)
        {
                       //***********fUNTION FOR COLOR CYLER PROGRAM*************
           // color_cycler_funtion(byteFrameNumber);
            //*********************************
            
            //********FUNTION WRITE COLOR************************
            writePacCol(wheel(byteFrameNumber));
            //***********************************
            
            delay(PERIOD);

        }
            

    
    }
    
    
    return 0;

}


