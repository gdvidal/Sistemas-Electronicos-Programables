/*
 * lab4_task2.c
 *
 * Created: 10-09-2019 16:17:36
 * Author : Guillermo
 */ 


#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


// Have a look at both the .h and the .c file, there is code missing.
#include "USART/USART_implementacion.h"


int main(void)

{
	//declaramos el struct 
												    //baud, size, parity, stop
	struct USART_configuration config_57600_8N1 = {57600,8,0,1};
	struct USART_configuration config_9600_8N2 = {9600,8,0,2};
	struct USART_configuration config_19200_7N1 = {19200,7,0,1};
	struct USART_configuration config_19200_7E2 = {19200,7,2,2};
	
	// Initialise the serial communication interface and print a test string
	USART_Init(config_57600_8N1);
	USART_Transmit_String("Congratulations! You are now sending test messages at 57600 8N1.\r\n");

	// Initialise the serial communication interface and print a test string
	USART_Init(config_9600_8N2);
	USART_Transmit_String("Change your terminal settings and reset the MCU to see 9600 8N2\r\n");

	// Initialise the serial communication interface and print a test string
	USART_Init(config_19200_7N1);
	USART_Transmit_String("With the wrong settings, you will see nothing but gibberish at 19200 7N1\r\n");

	// Initialise the serial communication interface and print a test string
	USART_Init(config_19200_7E2);
	USART_Transmit_String("Even parity and two stop bits have limited use. But 19200 7E2 is great for annoying students\r\n");


	// Play dead
	while(1);
	
}
