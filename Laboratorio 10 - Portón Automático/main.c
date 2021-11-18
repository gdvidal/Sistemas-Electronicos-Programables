#include <msp430.h>
#include "lcd.h"
#include <string.h>
#include <stdio.h>

#define cerrado  0b000
#define andando  0b001
#define abierto  0b010

#define detenido 0b011
#define error    0b100

#define timer_20_seg 3276
#define timer_5_seg 65535
#define parpadeo 0xFFFF;

volatile int estado= 0b000; /// 0: cerrado, 1: abierto, 2:andando, 3 detenido, 4:error


void timer_init(void){

    //timers to 16 bits

    // Parpadeo
    TA0CTL = TBSSEL_2 | TACLR | MC_1;
    TA0CCR0 = 0;
    TA0CCTL0 = CCIE; // Habilita interrupci�n (bit CCIE)

    // TimerA1: 20 seg
    TA1CTL = TASSEL_1 | TACLR | MC_1;
    TA1CCR0 = 0;
    TA1CCTL0 = CCIE; // Habilita interrupci�n (bit CCIE)

    //Timer B0: error
    TB0CTL = TBSSEL_2 + MC_1;
    TB0CCR0 = 65535;
    TB0CCTL0 = CCIE;

    // LPM0, habilitaci�n general de interrupciones
     __low_power_mode_0();
     __no_operation();
}

void I_O (void)
{
    // Led verde y rojo
    P6DIR |= (BIT1 | BIT2 | BIT3);
    P6OUT &=~ BIT1 ; //ROJO
    P6OUT &=~ BIT2 ; //AZUL
    P6OUT &=~ BIT3 ; //VERDE

    //PORTON
    P4DIR |= (BIT1 | BIT2);

    P4OUT &=~ BIT1; //verde
    P4OUT &~ BIT2;  //rojo


    // Button, switch1 y 2
    P2DIR &= ~(BIT3 | BIT4 | BIT5);
    // Activo interrupt boton
    P2IE |= (BIT3 | BIT4 | BIT5);
    // En flanco negativo
    P2IES |= (BIT3 | BIT4 | BIT5);

}

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    // Init: port, timer, lcd
    I_O();
    timer_init();

    lcdInit();

    lcd_cmd(0x80); // select 1st line (0x80 + addr) - here addr = 0x00
    lcdSetText("Cerrado!");

    lcd_cmd(0xc0); // select 2nd line (0x80 + addr) - here addr = 0x40
    lcdSetText("__________");

    __bis_SR_register(GIE); //to button


    while(1);

}

volatile int debouncing=0;
volatile int db_sw=0;

volatile int state_now= cerrado;   //parte cerrado
volatile int state_prev =-1;

volatile int bol=0;               //booleano para indicar cuando se presiona el boton
volatile int time_out=0;
volatile int count=0;
volatile int count_B=0;

volatile int var=0;


#pragma vector= PORT2_VECTOR
__interrupt void PORT2 (void)
{
    if (!(P2IN & BIT3))            //button: cambio de estado
    {
        debouncing++;

        if (debouncing >= 5)
        {
            bol=1;
            //count=0;
            //cambiar de estado

            //revision de estado previo:
            if (state_now == abierto)
            {
                state_prev= abierto;

            }

            else if (state_now == cerrado)
            {
                state_prev= cerrado;
            }

            else if (state_now == andando)
            {

                state_prev= andando;
                state_now = detenido;

            }

            debouncing=0;

        }

    }

    //// sw

    if ((P2IN & BIT4) && (!(P2IN & BIT5)))  //derecha: cerrado
    {
        db_sw++;

        if (db_sw >= 2)
        {
            if (bol ==1)
            {

                if (state_now ==error)
                {
                    P4OUT &=~ BIT1;
                    P4OUT &=~ BIT2;

                    lcdClear();
                    lcd_cmd(0x80);
                    lcdSetText("Error!!!");

                    TA0CCR0 = parpadeo;
                }

                else
                {
                    P4OUT |=BIT1;
                    P4OUT &=~BIT2;

                    lcdClear();
                    lcd_cmd(0x80);
                    lcdSetText("Abriendo");

                    //comienzo a contar para el error:
                    TB0CCR0 = timer_5_seg;

                }

            }


            else
            {
                P4OUT &=~BIT1;
                P4OUT &=~BIT2;
                state_now =cerrado;

                lcdClear();
                lcd_cmd(0x80);
                lcdSetText("Cerrado!!!");

                TA1CCR0 =0;
                TB0CCR0 =0;
                TA0CCR0 =0;

                count=0;
                count_B=0;
            }

            db_sw=0;


        }

    }

    else if (((P2IN & BIT5) && (!(P2IN & BIT4))) )  //izquierda:abierto
    {
        db_sw++;

        if (db_sw >= 2)
        {

            if (bol == 1)
            {

                if (state_now == error)
                {
                    P4OUT &=~ BIT1;
                    P4OUT &=~ BIT2;

                    lcdClear();
                    lcd_cmd(0x80);
                    lcdSetText("Error!!!");





                    TA0CCR0 = parpadeo;

                }

                else
                {
                    P4OUT &=~BIT1;
                    P4OUT |= BIT2;
                    TA1CCR0 =0;

                    lcdClear();
                    lcd_cmd(0x80);
                    lcdSetText("Cerrando");

                    //comienzo a contar para el error:
                    TB0CCR0 = timer_5_seg;

                }

            }

            else
            {
                state_now =abierto;

                //lcdClear();
                //lcd_cmd(0x80);
                //lcdSetText("Abierto!");

                TA1CCR0 = timer_20_seg;
                TB0CCR0 =0;
                TA0CCR0 =0;
                count_B=0;
            }
            db_sw=0;

        }


        //si pasan 20 seg. debe cerrarse

    }

    else if (((P2IN & BIT5) && ((P2IN & BIT4))) )  //al medio: motor andando (abriendo ó cerrando)
    {
        db_sw++;

        if (db_sw >= 3)
        {
            TB0CCR0 =0;
            TA0CCR0 =0;

            if (bol ==1)
            {
                state_now = andando;
                var++;

                bol=0;
            }

            if ((var <6) && (var>=3))
            {
                state_now = detenido;

            }

            if ((var >= 5) && (var <= 50))
            {
                state_now = andando;
                var=0;
            }


            if ((state_prev == cerrado) && (state_now == andando))
            {
                P4OUT |= BIT1; // led verde: motor funcionando en apertura
                P4OUT &=~BIT2;

                lcdClear();
                lcd_cmd(0x80);
                lcdSetText("Abriendo");
            }

            else if ((state_prev == abierto) && (state_now == andando))
            {
                P4OUT |= BIT2; // led rojo: motor funcionando en cierre
                P4OUT &=~BIT1;

                lcdClear();
                lcd_cmd(0x80);
                lcdSetText("Cerrando");
            }


            else if ((state_prev == andando) && (state_now == andando))
            {
                //si esta andando y se presiona el boton, comienza a cerrarse
                // esperar 20 seg.

                TA1CCR0 = timer_20_seg;

                //lcdClear();
                lcd_cmd(0x80);
                lcdSetText("Detenido!");
            }

            else if ((state_prev == detenido) && (state_now == andando))
            {
               P4OUT |= BIT2; // led rojo: motor funcionando en cierre
               P4OUT &=~BIT1;

               lcdClear();
               lcd_cmd(0x80);
               lcdSetText("Cerrando");

            }

            else if ((state_prev == andando) && (state_now == detenido))
            {
               P4OUT |= BIT2;
               P4OUT |=BIT1;

               lcdClear();
               lcd_cmd(0x80);
               lcdSetText("Det. indf");
               lcd_cmd(0xc0); // select 2nd line (0x80 + addr) - here addr = 0x40
               lcdSetText("__________");

               TA1CCR0 =0;

            }


            db_sw=0;

        }
    }

}

volatile int bol_2=0;

// timer 20 seg
#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER_A1(void)
{
    P6OUT ^=  BIT1;

    if (count >=201)      //pasan 20 seg: Porton debe cerrarse.
    {

       lcdClear();
       lcd_cmd(0x80);
       lcdSetText("Cerrando");

        P4OUT &=~ BIT1;
        P4OUT |= BIT2;


        if (state_now == abierto)        //cierre automatico
        {
            state_prev= abierto;
            state_now = andando;

        }

        TA1CCR0 =0;

    }

    else           // time <20 seg. Motor detenido
    {

        if (state_now == abierto)
        {
            lcdClear();
           lcd_cmd(0x80);
           lcdSetText("Abierto!");
        }

        P4OUT &=~ BIT1;
        P4OUT &=~ BIT2;
        count++;

    }


}


//timer to error.
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{
    P6OUT ^= BIT2;

    if (count_B >=77) //aprox. 5 seg.
    {
        state_now =error;
    }

    else
    {
        count_B++;
    }

}

volatile int count_p=0;
//timer to parpadeo
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER_A0(void)
{
    count_p++;

    if (count_p >= 80)
    {
        P4OUT &=~ BIT1;
        P4OUT &=~ BIT2;

    }

    else
    {
        P4OUT ^= BIT1;
        P4OUT ^= BIT2;



    }


}

