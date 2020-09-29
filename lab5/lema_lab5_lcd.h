

#ifndef LEMA_LAB5_LCD_H
#define	LEMA_LAB5_LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

    void lem_wait_1ms(void);//for delay function
    //WEEK 1- LAB5
    void lcd_cmd(char command);
    void lcd_init(void);
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    
      //WEEK 2- LAB5
    void lcd_printStr(const char *s);

#ifdef	__cplusplus
}
#endif

#endif	/* LEMA_LAB5_LCD_H */

