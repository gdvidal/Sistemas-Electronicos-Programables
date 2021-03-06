/***************************************************
 * This is a SPI library for the ATMega328P.
 *
 * It can provide basic SPI master functionality for any application, but is
 * directly targeted at the Adafruit 1.8" SPI display and its SD card socket.
 * 
 * It is based on example codes from the Atmel device datasheet. The code is
 * kept simple, but is not always efficient. It is an easy-to-understand example
 * for studies and not intended for productive use.
 *
 * This code is in the public domain. Feel free to do with it whatever you want.
 * 
 * 
 * 
 * FOR STUDENTS:
 * 
 * This file is complete, but the according .c file will be given to you in an
 * 'empty' state. See there for further information.
 * 
 ****************************************************/

#ifndef _SPI_IMPLEMENT_ME_H_
#define _SPI_IMPLEMENT_ME_H_

#include <stdint.h>
#include <stdbool.h>



// The initialisation function. Call it once from your main() program before
// issuing any SPI commands with the functions below!
void SPI_Master_Init(void);

#define wc(DATA) SPI_Master_transmit_char_display(DATA, true)
#define wd(DATA) SPI_Master_transmit_char_display(DATA, false)

// The transmit function. Takes one character and transmits it over SPI.
// Note that it does NOT control the Chip Select (CS) line!
void SPI_Master_transmit_char_display(uint8_t data, bool commandmode);

void SPI_Master_transmit_char(uint8_t data);
// This function receives a single byte over the SPI bus.
char SPI_Master_receive_char(void);


#endif // _SPI_IMPLEMENT_ME_H_