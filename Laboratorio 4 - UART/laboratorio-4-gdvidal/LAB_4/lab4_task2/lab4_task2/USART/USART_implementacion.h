/*
 * USART_implementacion.h
 *
 * Created: 10-09-2019 16:18:55
 *  Author: Guillermo
 */ 


#ifndef USART_IMPLEMENTACION_H_
#define USART_IMPLEMENTACION_H_

struct USART_configuration
{
	//contendio de variables struct
	uint16_t baud;
	uint8_t size;
	uint8_t parity;
	uint8_t stop;
	
};


// Call once to initialise USART communication
uint8_t USART_Init(struct USART_configuration config);

// Transmits a single character
void USART_Transmit_char(uint8_t data );

// Transmits a given string
void USART_Transmit_String(char* string);


#endif /* USART_IMPLEMENTACION_H_ */