
#ifndef LEMAC_LAB4_BUTTON_H
#define	LEMAC_LAB4_BUTTON_H

#ifdef	__cplusplus
extern "C" {
#endif
    
    
void initPushButton();
void __attribute__((__interrupt__, __auto_psv__)) _T2Interrupt(void);
void doubleDETECTOR();



#ifdef	__cplusplus
}
#endif

#endif	/* LEMAC_LAB4_BUTTON_H */

