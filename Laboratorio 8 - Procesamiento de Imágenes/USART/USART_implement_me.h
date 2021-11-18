/*
 * USART_implement_me.h
 *
 * Created: 22-10-2019 22:01:42
 *  Author: Guillermo
 */ 


#ifndef USART_IMPLEMENT_ME_H_
#define USART_IMPLEMENT_ME_H_

#include <stdint.h>

struct USART_configuration
{
	uint16_t baudrate;
	uint8_t databits;
	uint8_t parity;
	uint8_t stopbits;
};

struct ring_buffer
{
	char *buffer; //data buffer: almacenará char
	int size_buffer;
	int count_add; //num de items agregados al buffer
	int head;     //head to write
	int tail;     //tail to read

	int count_read;
};


//typedef struct RingBuffer; //dar un nuevo nombre a un tipo, en vez de llamar todo el tiempo struct linked_list ahora, solo usamos LinkedList

// Call once to initialise USART communication
void USART_Init(struct USART_configuration config);

// Transmits a single character
void USART_Transmit_char(uint8_t data );

// Transmits a given string
void USART_Transmit_String(char* string);

// Receives a single character
char USART_Receive_char(void);

// Receives a '\n' terminated string and writes it into a supplied buffer.
// The buffer must be guaranteed to handle at least bufflen bytes.
// Returns the number of bytes written into the buffer (If you want)
void USART_Receive_String(char* string, uint8_t largo);

//function to ring buffer

void bufferInit(struct ring_buffer *cbuffer, int size);

void buffer_push(struct ring_buffer *cbuffer, char letra);

char buffer_read(struct ring_buffer *cbuffer);

void bufferFree(struct ring_buffer *cbuffer);


#endif /* USART_IMPLEMENT_ME_H_ */