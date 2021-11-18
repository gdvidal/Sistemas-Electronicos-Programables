/*
 * task.c
 *
 * Created: 28-04-2019 19:46:52
 * Author : Rocio
 */ 
#define F_CPU	16000000UL

#define size_rx 5
#define size_tx 20

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "USART_implement_me.h"

char punto = 0b101110;	//punto en bin
volatile caso;

struct ring_buffer buffer_rx;
struct ring_buffer buffer_tx;


int main(void)
{
	
	cli();
	DDRB &= ~(1<<DDB7);			// PB7 como entrada
	PCICR |= (1 << PCIE0);     	// habilitamos de PCINT[7:0]
	PCMSK0 |= (1 << PCINT7);   	// selecionamos solo los cambios de PCINT7
	
	DDRB |= (1 << 5); // habilitar led para guiarse
	
	struct USART_configuration config_57600_8N1 = {57600, 8, 0, 1};
		
	USART_Init(config_57600_8N1);

	bufferInit(&buffer_rx,size_rx);
	bufferInit(&buffer_tx, size_tx);
	
	sei();
	
	while(1){
		
	}
}

ISR (PCINT0_vect)
{	/* cambio PCINT0 */
	char * string_1 ="El boton fue soltado";
	char *string_2 ="El boton fue soltado";
	
	if(PINB & (1<<7))
	{
		
		/* LOW to HIGH pin change */
		/*
		while(*string != 0x00)
		{
			buffer_push(&buffer_rx,string_1*)
			string_1++;
		}
		
		
		UCSR0B &= ~(1<<RXCIE0);
		UCSR0B |= (1<<UDRIE0);
		*/
		USART_Transmit_String("El boton fue soltado.\r\n");
	}

	else
	{
		
		
		/* HIGH to LOW pin change */
		USART_Transmit_String("El boton esta siendo pulsado.\r\n");
	}
	
	
	
}



ISR (USART_RX_vect) // INTERRUPCION DE RECEPCION
{	
	char letra;
	letra=  UDR0;
	
	//si la letra es un punto, se termina de recibir y se habilita la transmision
	if (letra == punto)
	{
		UCSR0B &= ~(1<<RXCIE0);
		UCSR0B |= (1<<UDRIE0);
		
	}
		
	else
	{
		buffer_push(&buffer_rx, letra);	
			
	}
	
}

ISR (USART_UDRE_vect)
{	
	char l;	
	caso++;
	
	//leemos el buffer_rx y guardamos esto en el buffer_tx
	if (caso ==1)
	{
		if (buffer_rx.count_add>buffer_rx.size_buffer)
		{
			for (int i=0; i< buffer_rx.size_buffer;i++)
			{
				l=buffer_read(&buffer_rx);
				buffer_push(&buffer_tx, l);

			}
		}
	
		else
		{
			for (int i=0; i<buffer_rx.count_add;i++)
			{
				l=buffer_read(&buffer_rx);
				buffer_push(&buffer_tx, l);
			}
		
		}
		
	}
	
	////////// ahora transmitimos esto /////////////
	
	if ((buffer_tx.tail == buffer_tx.head) && (buffer_tx.head !=0) && (buffer_tx.tail!=0))
	{
		bufferInit(&buffer_rx,size_rx);
		bufferInit(&buffer_tx,size_tx);
		caso=0;
		UDR0 = '\n\r';
		//habilito la recepecion quedando listo para recibir un nuevo mensaje.
		UCSR0B &= ~(1<<UDRIE0);
		UCSR0B |= (1<<RXCIE0);
		
	}
	
	else if (buffer_tx.count_read == buffer_tx.size_buffer)
	{
		bufferInit(&buffer_rx,size_rx);
		bufferInit(&buffer_tx,size_tx);
		caso=0;
		UDR0 = '\n\r';
		//habilito la recepecion quedando listo para recibir un nuevo mensaje.
		UCSR0B &= ~(1<<UDRIE0);
		UCSR0B |= (1<<RXCIE0);
	}
	
	
	
	else
	{
		l=buffer_read(&buffer_tx);
		UDR0= l;
		_delay_ms(5);	
	}
	
}