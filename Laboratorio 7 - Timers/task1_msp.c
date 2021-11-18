
#include <msp430.h> 
#include <stdint.h>

void display_(int numero, int display);
void TIMER_init(void);

void display(int num, int dis){
   // pines: 1.2 1.3 1.4
    switch(dis){
        case 1 : P1OUT |= (1<<2) ; P1OUT &= ~(1<<4); P1OUT &= ~(1<<3); break;
        case 2 : P1OUT |= (1<<3) ; P1OUT &= ~(1<<2); P1OUT &= ~(1<<4); break;
        case 3 : P1OUT |= (1<<4) ; P1OUT &= ~(1<<3); P1OUT &= ~(1<<2); break;


        }

    switch(num){
                          // gfedcb0a
        case 0: P2OUT = (~(0b01111101)); break;
        case 1: P2OUT = (~(0b00001100)); break;
        case 2: P2OUT = (~(0b10110101)); break;
        case 3: P2OUT = (~(0b10011101)); break;
        case 4: P2OUT = (~(0b11001100)); break;
        case 5: P2OUT = (~(0b11011001)); break;
        case 6: P2OUT = (~(0b11111001)); break;
        case 7: P2OUT = (~(0b00001101)); break;
        case 8: P2OUT = (~(0b11111101)); break;
        case 9: P2OUT = (~(0b11011101)); break;   }
}


void timer_init(void){
    // Configuraci�n TIMER_A1:
    // TimerA1, ACLK/1,modo up, reinicia TACLR
    TA1CTL = TASSEL_1 | TACLR | MC_1;

    //TASSEL = 1 ACLK
    //MC_1 =  UP mode
    //TACLR = Timer_A clear. Setting this bit clears TAR, the clock divider logic (the divider
    //setting remains unchanged), and the count direction. The TACLR bit is
    //automatically reset and is always read as zero.


    // ACLK tiene una frecuencia de 32768 Hz
    // Carga cuenta en TA1CCR0 0.1seg TA1CCR=(0,1*32768)-1
    //taxccr0 + 1 num de cuentas del timer en el periodo
    //cuando el valor del timer es igual a taxccr0 el timer se reinicia contando desde 0.

    TA1CCR0 = 3276;
    //TAXCCTL capture/compare control regitrer
    TA1CCTL0 = CCIE; // Habilita interrupci�n (bit CCIE)
    // Capture/compare interrupt enable. This bit enables the interrupt request of the
    //corresponding CCIFG flag.
    //0b = Interrupt disabled
    //1b = Interrupt enabled
    // LPM0, habilitaci�n general de interrupciones


    // Configuraci�n TIMER_A0:

    // TimerA1, ACLK/1,modo up, reinicia TACLR
    TA0CTL = TASSEL_1 | TACLR | MC_1;
    // ACLK tiene una frecuencia de 32768 Hz
    // Carga cuenta en TA1CCR0 0.1seg TA1CCR=(0,1*32768)-1
    TA0CCR0 = 200;
    TA0CCTL0 = CCIE; // Habilita interrupci�n (bit CCIE)

    // LPM0, habilitaci�n general de interrupciones
     __low_power_mode_0();
     __no_operation();

}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    P2DIR |= (0b11111101); //GFED_CB0A
    P1DIR |= (0b01011100); // digitos 1 2 3

    P1DIR |= BIT0;               // P1.0 salida
    P1OUT |= BIT0;               // Enciende LED

//    P1IE | = (0b00011100) ;                    // P1.3 interrupci�n habilitada
//    P1IES | = (0b00011100);                  // P1.3 Hi / lo edge
//    P1IFG & = ~(0b00011100);               // P1.3 IFG borrado
    timer_init();

    while(1);
    return 0;
}


// Rutina de interrupci�n de TIMER1_A0

int num1 = 0;
int num2 = 0;
int num3 = 0;
int dis = 1;

volatile unsigned i;

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_ISR(void){

  P1OUT ^= 0x01;       // conmuta LED en P1.0

//  display( num1,  1);
  num1++;
  if(num1 == 10){
      num1 = 0;
      num2++;
  }

//  display( num2,  2);

  if(num2 == 10){
      num2 =0;
      num3 ++;
  }

//  display( num3,  3);

  if (num3 == 10){
      num1 =0;
      num2 =0;
      num3 =0;
  }

}


#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void){

    if(dis == 1){
        display( num1,  dis);
        P1OUT |= BIT6;
    }

    if(dis == 2){
            display( num2,  dis);
            P1OUT &= ~BIT6;

        }

    if(dis == 3){
            display( num3,  dis);
            P1OUT |= BIT6;
        }

    dis++;
    if (dis == 4){
        dis = 1;
    }

}
