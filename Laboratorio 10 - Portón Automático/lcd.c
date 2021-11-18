/*
 * lcd.c
 *
 *  Created on: 27-11-2019
 *      Author: Guillermo
 */

#include "lcd.h"
#include <msp430.h>

/* Build: msp430-gcc -mmcu=msp430g2231 -g -o lcd.elf lcd.c */

/* uC and LCD Connections
    TP1 - Vcc (+5v)
    TP3 - Vss (Gnd)
    P1.0 - D4
    P1.1 - D5
    P1.2 - D6
    P1.3 - D7
    P1.4 - EN
    P1.5 - RS
    Gnd  - RW
    Gnd  - Vee through 1K Resistor  - this value determines contrast - i.e. direct connection to Gnd means all dots displayed
    Gnd  - K (LED-)
    Vcc  - A (LED+) +5V
    Clock: 1MHz             */


// uC Port definitions
#define lcd_port        P3OUT
#define lcd_port_dir    P3DIR

// LCD Registers masks based on pin to which it is connected
#define LCD_EN      BIT4
#define LCD_RS      BIT5

void lcdClear()
{
    lcd_port_dir = 0xff;    // output mode
    lcd_port = 0xff;
    __delay_cycles(20000);
    lcd_port = 0x03+LCD_EN;
    lcd_port = 0x03;
    __delay_cycles(10000);
    lcd_port = 0x03+LCD_EN;
    lcd_port = 0x03;
    __delay_cycles(1000);
    lcd_port = 0x03+LCD_EN;
    lcd_port = 0x03;
    __delay_cycles(1000);
    lcd_port = 0x02+LCD_EN;
    lcd_port = 0x02;
    __delay_cycles(1000);
}

void lcd_cmd (char cmd)
{
    // Send upper nibble
    lcd_port = ((cmd >> 4) & 0x0F)|LCD_EN;
    lcd_port = ((cmd >> 4) & 0x0F);

    // Send lower nibble
    lcd_port = (cmd & 0x0F)|LCD_EN;
    lcd_port = (cmd & 0x0F);

    __delay_cycles(4000);
}

void lcdInit ()
{
    lcdClear();         // Call LCD reset
    lcd_cmd(0x28);       // 4-bit mode - 2 line - 5x7 font.
    lcd_cmd(0x0C);       // Display no cursor - no blink.
    lcd_cmd(0x06);       // Automatic Increment - No Display shift.
    lcd_cmd(0x80);       // Address DDRAM with 0 offset 80h.
    lcd_cmd(0x01);       // Clear screen
}


void lcd_data (unsigned char dat)
{
    // Send upper nibble
    lcd_port = (((dat >> 4) & 0x0F)|LCD_EN|LCD_RS);
    lcd_port = (((dat >> 4) & 0x0F)|LCD_RS);

    // Send lower nibble
    lcd_port = ((dat & 0x0F)|LCD_EN|LCD_RS);
    lcd_port = ((dat & 0x0F)|LCD_RS);

    __delay_cycles(4000); // a small delay may result in missing char display
}

void lcdSetText(char *line)
{
    while (*line)
        lcd_data(*line++);
}
