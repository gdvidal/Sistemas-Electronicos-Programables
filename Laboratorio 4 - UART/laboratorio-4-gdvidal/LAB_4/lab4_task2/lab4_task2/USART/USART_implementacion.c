/*
 * USART_implementacion.c
 *
 * Created: 10-09-2019 16:18:36
 *  Author: Guillermo
 */ 

#define F_CPU	16000000UL

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>

#include "USART_implementacion.h"


uint8_t USART_Init(struct USART_configuration config)
{
	//usart como uart
	UCSR0C &=~ (1<<UMSEL00);
	UCSR0C &=~ (1<<UMSEL01);
	
	
	//Velocidad de la comunicacion
	
	uint16_t ubrr=F_CPU/16/config.baud-1;
	
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	//size//
	switch(config.size)
	{
		case 5:
		UCSR0C &= ~((1 << UCSZ01) | (1 << UCSZ00));
		break;
		
		case 6:
		UCSR0C |= (1 << UCSZ00);
		break;
		
		case 7:
		UCSR0C |= (1 << UCSZ01);
		break;
		
		case 8:
		UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00));
		break;
	}
	
	// Parity //
	switch(config.parity) {
		case 0: // No parity
		UCSR0C &= ~((1 << UPM00) | (1 << UPM01));
		break;
		case 1: // Odd parity
		UCSR0C |= ((1 << UPM01) | (1 << UPM00));
		break;
		case 2: // Even parity
		UCSR0C |= (1 << UPM01);
		break;
	}
	
	//stop//
	switch(config.stop) {
		case 1:
		UCSR0C &= ~(1 << USBS0); //unico bit de stop
		break;
		case 2:
		UCSR0C |= (1 << USBS0); //2 bits de stop
		break;
	}
	
	// Transmit Enable: pin TX//
	UCSR0B |= (1<<TXEN0);
	UCSR0B |= (1<<RXEN0);
	_delay_ms(100);
}


void USART_Transmit_Char(unsigned char data)
{
	while(!(UCSR0A & (1<<UDRE0))); //minetras no reciba un dato
	//cargamos buffer(donde almancenamos dato)
	//irve para leer y transmitir
	UDR0 = data; //espacio en memori que almacena dato de manera temporal
}



// Transmits a given string
void USART_Transmit_String(char* string)
{
	while(*string != 0x00) { //ultima direccion es 0
		USART_Transmit_Char(*string);
		string++;
	}
}

