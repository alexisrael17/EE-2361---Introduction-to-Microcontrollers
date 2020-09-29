
#include "xc.h"
#include "lema_lab5_lcd.h"

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


void setup()
{
    CLKDIVbits.RCDIV = 0; //8MHz//gives the builtin 8mhz oscillator, not divided by a postscaler, and
    //finally divided by two = 4MHz. 
    AD1PCFG = 0xffff; //all digital inputs and outputs// Initialize AN pins as digital
    I2C2BRG = 157;  //BaZZZud Rate Generator Register//Table 16-1 page 153/268 FAMILYDATASHEET.PDF
    I2C2CONbits.I2CEN = 1; //Enable
                          //1 = Enables the I2Cx module and configures the SDAx and SCLx pins as serial port pins
                        //0 = Disables the I2Cx module; all the I2C? pins are controlled by port functions
    IFS3bits.MI2C2IF=0; //Set when an I2C action is complete //AFTER "_I2CSIDL = 0; "
 //   T1CON=0
    lcd_init();    //CALL lcd_init(); located in lem_lab5_lcd.c
    
 
}

void delay(int delay_in_ms) // delay in miliseconds 
{
    while(delay_in_ms>0)
    {
      lem_wait_1ms(); // this funtion works with assembly file and header 
      delay_in_ms--; // counter/ decrement 
    }//end while(delay_in_ms>0) 
}//end delay

int main()
{
    setup(); //CALL setup(); located in lema_lab5_main.c
    
    
    while (1) 
    {  
      /////<<<<<<<WEEK 1>>>>>>>>>>>    ///////
     // lcd_setCursor(2,1);   //x///1-7    //COLUMN
                             //y//1-2     //
	 // lcd_printChar('ALEX');              //

    lcd_setCursor(0,0);        
    lcd_printStr("HELLO");  
    lcd_setCursor(3,1);
    lcd_printStr("WORLD!");
    lcd_cmd(0b00011100);
    delay(100);
    /////////////////////////
    } 
    return 0;
}
































      /////<<<<WEEK 2>>>>>>>>>>>>>>///////
      ///<<SHIFTING TO THE LEFT>//////////
      //   lcd_cmd(0x18);            /////
      ////////////////////////////////////
      ///<<SHIFTING TO THE RIGHT>/////////
       //     lcd_cmd(0x1C);              //
      ////////////////////////////////////
       //   delay(350);                   //
      //////////////////////////////////*/