/*
 * USART_implement.h
 *
 *  Created on: 10-09-2019
 *      Author: Guillermo
 */

#ifndef USART_IMPLEMENT_H_
#define USART_IMPLEMENT_H_

#include <stdint.h>


// Call once to initialise USART communication
void USART_Init();

// Transmits a single character
void USART_Transmit_char(uint8_t data );


#endif /* USART_IMPLEMENT_H_ */

