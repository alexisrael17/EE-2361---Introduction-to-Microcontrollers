/* 
 * File:   lema_lab3_header.h
 * Author: Alex
 *
 * Created on July 1, 2019, 11:19 AM
 */

#ifndef LEMA_LAB3_DISPLAYHEADER_H
#define	LEMA_LAB3_DISPLAYHEADER_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define MSB  0b0000010000000000
#define LSB  0b0000100000000000

void lemaTest7seg(void); 
void init7seg(void);
void showChar7seg(char myChar, int myDigit);


#ifdef	__cplusplus
}
#endif

#endif	/* LEMA_LAB3_HEADER_H */

