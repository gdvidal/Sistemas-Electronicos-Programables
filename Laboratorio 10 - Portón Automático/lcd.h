/*
 * lcd.h
 *
 *  Created on: 27-11-2019
 *      Author: Guillermo
 */

/*********************************************************************

LCD Driver Functions

by: Elliott Gurrola

    LCD Pinouts

    Pin 1   Ground
    Pin 2   VCC (+3.3 to +5V)
    Pin 3   Contrast adjustment
    Pin 4   Register Select (RS). 0: Command, 1: Data
    Pin 5   Read/Write (R/W). 0: Write, 1: Read     -> Set to GND for constant write
    Pin 6   Clock (Enable). Falling edge triggered
    Pin 7   Bit 0 (Not used in 4-bit operation)
    Pin 8   Bit 1 (Not used in 4-bit operation)
    Pin 9   Bit 2 (Not used in 4-bit operation)
    Pin 10  Bit 3 (Not used in 4-bit operation)
    Pin 11  Bit 4
    Pin 12  Bit 5
    Pin 13  Bit 6
    Pin 14  Bit 7
    Pin 15  Backlight Anode (+)
    Pin 16  Backlight Cathode (-)

    Top level functions available
        - lcd_init();                   // Initialize LCD
        - write_byte(char l);           // Send a single character (1 byte)
        - write_string(char s[]);       // Print a character string
        - write_int(int num);           // Write an integer
        - gotoXy(x, y);                 // Move cursor to (x, y) location (Lines and columns start at 0)
        - clear_lcd();                  // Clear LCD and move cursor to (0, 0)

    Not important to the user:
        - send_cmd(char cmd);           // Send a command to the LCD
        - write_nibble(char l);         // Send a nibble to the LCD
        - void trigger_EN();

    *** DELAY FUNCTION ASSUMES 1 MHz CLOCK ***


Modified on: Apr 15, 2013
by: Luiz (Luis Carlos Bauelos-Chacon)

Modified on: May 24, 2013
by: Elias N Jaquez
*********************************************************************/

#ifndef LCD_H_
#define LCD_H_

#include <msp430.h>
#include <string.h>
#include <stdio.h>

void lcdClear(void);
void lcd_cmd (char cmd);
void lcd_data (unsigned char dat);
void lcdInit (void);
void lcdSetText(char *line);







#endif /* LCD_H_ */
