

/*
#include <msp430.h> 
#include <inttypes.h>

int T=30000;

void main(){                    //funcion principal

    WDTCTL= WDTPW+WDTHOLD;      //Apagamos el watchdog

    P1SEL= 0x00;
    P1DIR|= BIT0;               //Salida el bit 0 del puerto 1

    P1OUT|=BIT0;                //LED parte apagado

   // agregamos un led externo P2.5
    P2DIR|= BIT5;
    P2OUT |=BIT5; // PARTE APAGADO

    //AGREGAMOS BOTON S2
    P2DIR&=~ BIT1; // ENTRADA
    P2REN |= BIT1; //PULL DOWN PARA LA ENTRADA

    //habilitamos interrumpcion

    P2IE|=BIT1;       //Habilitamos las interrupciones, mas información en el userguide.
    P2IES|=BIT1;
    P2IFG&=~BIT1;

    _BIS_SR(GIE);


while(1){

    if (P2IN & BIT1){ //s2 no esta pulsado

        P1OUT|=BIT0;            //prende el LED
        P2OUT |=BIT5;

        __delay_cycles(1000000); //espera

        P1OUT&=~BIT0;           //apaga el LED
        P2OUT &=~BIT5;
        __delay_cycles(1000000); //espera

    }

    else {
        P1OUT|=BIT0;            //prende el LED
        P2OUT |=BIT5;
        __delay_cycles(500000); //espera

        P1OUT&=~BIT0;           //apaga el LED
        P2OUT &=~BIT5;
        __delay_cycles(500000); //espera
    }

}

}

__interrupt void Led_ISR (void){
    P1IFG&=~BIT3;                    //Para salir de una interrupción SIEMPRE es necesario limpiar la bandera.
    T=T-2000;
}
*/


////////////////////////////////////////////////7

/*
#include <msp430.h>
#include <inttypes.h>
int i=0;                          //Declaración de variables.
int T=30000;

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD; // Detiene el watchdog

  P1DIR |= BIT0;            // P1.0 (LED) como salida digital
  P2DIR |= BIT5;        // pin 2.5 como salida digital
  P1REN |= BIT1;        // Resistencia en P1.1 (pulsador derecho)
  P1OUT |= BIT1;        // Modo pull-up
  P1IE |= BIT1;         // habilita la interrupción de P1.1
  P1IES |= BIT1;        // Interrupción por flanco de bajada
  P1IFG &= ~(BIT1);     // baja flag de interrupción de P1.1

  // Activar interrupciones
  // también puede usarse: __enable_interrupt()
  __bis_SR_register(GIE);

  // bucle infinito. Poco eficiente energéticamente.
  // Ya lo mejoraremos.
  while (1)
    __no_operation();
}

#pragma vector=PORT1_VECTOR
__interrupt
void P1_ISR(void) {

  if (P1IFG & BIT1) {   // comprobar fuente de la interrupción P1.1
    if (P1IN & BIT1) {  // Estado de P1.1: si está a 1 (no pulsado)

      P1OUT &= ~BIT0;   // Apaga LED
      P2OUT &= ~BIT5;   // LED PROTO APAGADO
      P1IES |= BIT1;    // activación por flanco de bajada

    } else {            // y si no (pulsado),

      P1OUT |= BIT0;    // Enciende LED
      P2OUT |= BIT5;    // LED PROTO ENCENDIDO
      P1IES &= ~BIT1;   // activación por flanco de subida
    }
    P1IFG &= ~BIT1; // puesta a cero del flag de interrupción P1.1
  }
}

*/

#include <msp430.h>

int i=0;
int T=30000;

void main(){


    WDTCTL = WDTPW + WDTHOLD; // Detiene el watchdog

    //El MSP430 y muchos otros microcontroladores incorporan un contador de seguridad llamado Watchdog timer. Su principal función es la de evitar que el chip se bloquee por haber entrado en un loop infinito por un error de programación. Este contador está permanentemente contando y si supera cierto umbral, hace que todo el microprocesador se reinicie, ejecutando el programa desde el inicio.
    //Por ahora, deshabilitaremos este contador para evitar que nuestros programas se reinicien.

    P1DIR |= BIT0;            // P1.0 (LED) como salida digital
    P2DIR |= BIT5;        // pin 2.5 como salida digital
    P1REN |= BIT1;        // Resistencia en P1.1 (pulsador derecho)
    P1OUT |= BIT1;        // Modo pull-up
    P1IE |= BIT1;         // habilita la interrupción de P1.1
    P1IES |= BIT1;        // Interrupción por flanco de bajada
    P1IFG &= ~(BIT1);     // baja flag de interrupción de P1.1

    // Activar interrupciones
    // también puede usarse: __enable_interrupt()
    __bis_SR_register(GIE);

while(1){
    for(i=0;i<T;i++);

    P1OUT^=BIT0;
    P2OUT^=BIT5;

    if(T<=2000){
        T=30000;

    }

}
}

#pragma vector= PORT1_VECTOR

__interrupt void Led_ISR (void){
    P1IFG&=~BIT1;                    //Para salir de una interrupción SIEMPRE es necesario limpiar la bandera.
    T=T-2000;
}
