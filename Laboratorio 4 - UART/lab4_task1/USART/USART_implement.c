/*
 * USART_implement.c
 *
 *  Created on: 10-09-2019
 *      Author: Guillermo
 */

#include <msp430.h>
#include <string.h>
#include "USART_implement.h"

// Esta funcion configura la comunicación USART con la tarjeta, ubrr es el baud rate.
void USART_Init()
{
    WDTCTL = WDTPW + WDTHOLD;

    //CONFIG PINES TX Y RX
    P4SEL |= BIT5 | BIT4;  //4.5 y 4.4

    //UCA1 in reset to be configured
    UCA1CTL1 |= UCSWRST;

    UCA1CTL1 |= UCSSEL_2; //SMCLK: source clock (internal clock)

    //CONFIGURE

    //definimos baudrate a 1.04 MHz

    UCA1BR0 = 109;
    UCA1BR1 = 0x00;// 0 pues no ocupamos este registro
    UCA1MCTL = UCBRS_2;//MODULATION UCBRSx =2

    //configurar USART como UART

    //UCA1CTL0 &=~ UCSYNC; //ASYNCHRONUS MODE

    //bit de stop
    //UCA1CTL0 &=~ UCSPB; //1 bits de stop
    //UCA0CTL0 |= UCSPB; //2 bits de stop

    //size bit

    //UCA1CTL0 &=~UC7BIT; //8bits
    //UCA0CTL0 |=UC7BIT; //7bits

    //paridad
    //UCA1CTL0 &=~UCPEN; //parity disabled

    // UCA0TL0 &=~ UCPAR (IMPAR)
    // UCA0TL0 |= UCPAR (PAR)
    UCA1CTL1 &= ~ UCSWRST; //INITIALITE USCI STATE MACHINE (out of reset)

    __delay_cycles(7500);
    //UCA1IE |= UCTXIE; //ENABLE USCI TX INTERRUPT
}



// Transmite solo un char, data corresponde al char a enviar
void USART_Transmit_char(uint8_t data)
{
    //while (!(UCA1IFG & UCTXIFG));
    while (UCA1STAT & UCBUSY); //mientras se encuentre ocupado
    UCA1TXBUF = data; //tx mando al buffer de transmision

}

