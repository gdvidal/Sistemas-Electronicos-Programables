/***************************************************
 * This is a USART library for the ATMega328P.
 *
 * It can provide basic USART (serial) communication for any application.
 *
 * This code is in the public domain. Feel free to do with it whatever you want.
 * 
 * 
 * 
 * FOR STUDENTS:
 * 
 * This file will be given to you in an 'empty' state. The function bodies are
 * provided, but not their content. You are supposed to add the proper code
 * and complete these functions.
 * 
 * Hint 1: RTFM! The Atmel device datasheets contain all information necessary.
 * 
 * Hint 2: We do not expect you to write the most performant or exceptionally
 * well-engineered code, but it should work. If in doubt, sacrifice speed for
 * reliability. If you are bored, rewrite the entire library in assembler and
 * squeeze the last microsecond out of it.
 * 
 ****************************************************/


// MCU Clock Speed - needed for baud rate value computation
#define F_CPU	16000000UL

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "USART_implement_me.h"

void USART_Init(struct USART_configuration config)
{
	uint16_t ubrr = F_CPU/16/config.baudrate - 1;
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	
	//size//
	switch(config.databits)
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
		//default: printf("Error");
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
		//default: printf("Error");
	}
	
	//stop//
	switch(config.stopbits) {
		case 1:
			UCSR0C &= ~(1 << USBS0);
			break;
		case 2:
			UCSR0C |= (1 << USBS0);
			break;
		//default: printf("Error");
	}
	
	// Transmit Enable //
	//UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);

	UCSR0B |= (1 << RXCIE0)|(1<<UDRIE0);
	_delay_ms(100);

}




// Transmits a single character
void USART_Transmit_char(uint8_t data)
{
	while(!(UCSR0A & (1 << UDRE0)));
	UDR0 = data;
	
}



// Transmits a given string
void USART_Transmit_String(char* string)
{
	while(*string != 0x00)
	{
		USART_Transmit_char(*string);
		string++;
	}

}




// Receives a single character
char USART_Receive_char(void)
{	
	while (!(UCSR0A & (1 << RXC0))); //RXC0 =1 cuando el recibo esta completado
	//se queda en el while mientras no se haya completado el recibo
	
	return UDR0; //returna buffer	
}


void USART_Receive_String(/*Fill here*/)
{
	char letra;
	char punto = 0b101110;	//punto en bin
	
	while(!(UCSR0A & (1 << RXC0))); //mientras no reciba nada se queda acá
	struct ring_buffer buffer;
	
	bufferInit(&buffer,20);
	
	//lo recibido es guardado en el buffer
	
	while (letra !=punto)
	{
			
		letra= USART_Receive_char();
		if (!(letra==punto))
		{
			buffer_push(&buffer, letra);
			
		}
				
	}
	
	//una vez almacenado el string en el buffer, lo transmitimos:
	USART_Transmit_String("I received this line: ");

	if (buffer.count_add>buffer.size_buffer)
	{
		for (int i=0; i< buffer.size_buffer;i++)
		{
			buffer_read(&buffer);
		}
	}
	
	else 
	{
		for (int i=0; i<buffer.count_add;i++)
		{
			buffer_read(&buffer);
		}
		
	}
	USART_Transmit_String(".\r\n\r\n");
}


void bufferInit(struct ring_buffer *cbuffer, int size)
{
	//definimos tamaño e inicializamos en 0 lo demás.
	cbuffer->size_buffer= size;
	cbuffer->head = 0;
	cbuffer->tail=0;
	cbuffer->count_add=0; 
	cbuffer->count_read=0;
	
	//asignacion dinamica de memoria con un tamaño fijo (malloc)
	cbuffer->buffer = (char*)malloc(sizeof (int) * size);
	
	
}



void buffer_push(struct ring_buffer *cbuffer, char letra)
{
			
	int next;
	next = cbuffer->head +1; //nos movemos una posicion
	
	//cond. buffer circular
	if (next >= cbuffer->size_buffer){
		next=0;
	}
	
	
	
	//load letra and then move allocation
	cbuffer->buffer[cbuffer->head] = letra;
	cbuffer->head = next;
	cbuffer->count_add ++;
	
	
}

char buffer_read(struct ring_buffer *cbuffer)
{
	int next;
	char read;
	
	if (cbuffer->count_add > cbuffer->size_buffer)
	{
		
		cbuffer->tail= cbuffer->head;
		cbuffer->count_add=0;
		
	}
	
	next = cbuffer->tail + 1; //point after read
	if (next >= cbuffer->size_buffer){
		next=0;
	}
	
	read= cbuffer->buffer[cbuffer->tail];
	//USART_Transmit_char(read);
	cbuffer->tail=next;
	cbuffer->count_read ++;
	
	return read;
		
}






