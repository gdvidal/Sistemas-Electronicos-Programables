/*
 * SPI_implement_me.c
 *
 * Created: 22-10-2019 22:01:07
 *  Author: Guillermo
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

#include "SPI_implement_me.h"

#define MOSI	3	//pin B
#define MISO	4	//pin B
#define SCK		5	//pin B

#define D_C		0
#define TFT_CS  1
#define CARD_CS	2  

#define LED 3

void SPI_Master_Init(void) {
	
	DDRB |= (1<<MOSI) | (1<<SCK) | (1<<TFT_CS) | (1<<CARD_CS) | (1<<D_C); //DC=0 command data, DC=1 display data
	DDRB &=~ (1<<MISO); //MOSI= SEND ; MISO= RECEIVE AT THE SAME TIME
	DDRD |= (1<<LED); 
	
	//SLAVES
	
	//PORTB |= (1<<D_C);
	//PORTB |= (1<< CARD_CS);
	//PORTB |= (1<<TFT_CS);
	
	//spi enable, master, clk fclk/16 =1mhz
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
	
	_delay_ms(1000);

}


void SPI_Master_transmit_char_display(uint8_t data, bool commandmode)
 {
	//PORTB &=~ (commandmode << D_C);
	if (commandmode)
	{
		PORTB &=~ (1<< D_C);
	}
	
	else
	{
		PORTB |= (1<<D_C);
	}
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
	
	PORTB |= (1 << SCK);
}

void SPI_Master_transmit_char(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

char SPI_Master_receive_char(void) {
	
	SPI_Master_transmit_char(0xFF); // push to generate clock from master
	while (!(SPSR & (1 << SPIF)));
	return SPDR;
}
