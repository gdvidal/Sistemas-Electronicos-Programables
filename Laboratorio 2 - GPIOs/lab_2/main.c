/*
 * lab_2.c
 *
 * Created: 29-08-2019 14:08:28
 * Author : Guillermo
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <avr/interrupt.h>
#define fclk 100 // frec a la que realiza interrupcion

//declaramos las funciones 
void timer_init();
void timer_on();
void timer_off();

void display_num(uint8_t numero);


#define N0 0b00000011
#define N1 0b10011111
#define N2 0b00100101
#define N3 0b00001101
#define N4 0b10011001
#define N5 0b01001001
#define N6 0b01000001
#define N7 0b00011111
#define N8 0b00000001
#define N9 0b00011001

#define D1 1
#define D2 2
#define D3 4

////////////timer ///////////////


void timer_init(){
	
	//Operation Mode ctc (comparación)

	//registro control

	TCCR1A &=~ (1<<WGM10);
	TCCR1A &=~ (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B &=~ (1<<WGM13);
	
	//definimos el registro con el valor que vamos a comparar

	OCR1A= (F_CPU/1024/fclk) -1;
	//OCR1A = ; //def la frec de interrumpcion según la division del clk

	//habilitamos la interrupcion, que se da c/vez que el contador alcanza OCR1A
	// 1 y interrupcion gen = 1, timer genera interrupcion por comparacion
	TIMSK1 |= (1<<OCIE1A);
	
}


void timer_on(){
	
	//def registro para acceder al timer 1. hago al contador 0 para que empiece a contar desde el cero
	TCNT1 = 0;
	
	// CS me indica en cuanto dividiremos la frec del clock
	//util para saber a cuánto vamos a contar
	
	TCCR1B |= (1<<CS10);
	TCCR1B &=~ (1<<CS11);
	TCCR1B |= (1<< CS12);

	// divide clk/1024
}


void timer_off(){
	//detenemos el clock. setting in 0Hz
	TCCR1A &=~ (1<<CS10);
	TCCR1A &=~ (1<<CS11);
	TCCR1B &=~ (1<< CS12);
}

void display_num(uint8_t numero)
{
	switch (numero) {
		
		case 0:
		PORTD = N0;
		break;
		
		case 1:
		PORTD = N1;
		break;
		
		case 2:
		PORTD = N2;
		break;
		
		case 3:
		PORTD = N3;
		break;
		
		case 4:
		PORTD = N4;
		break;
		
		case 5:
		PORTD = N5;
		break;
		
		case 6:
		PORTD = N6;
		break;
		
		case 7:
		PORTD = N7;
		break;
		
		case 8:
		PORTD = N8;
		break;
		
		case 9:
		PORTD = N9;
		break;}
	
}


////////////////////MAIN//////////////////////////////


int main (void)
{
	
	cli(); //desactivamos interrupciones generales
	DDRD= 0xFF; // puerto D como output
	DDRC = 0xFF; //CONF los anodos como salidas
	PORTC = 0b001;
	PORTD= 0x00;
	
	timer_init(); //inicializamos la interrupcion
	
	sei (); //activamos interrupciones generales
	PORTD = N0;
	timer_on(); //inicia timer a contar
		
	
	while(1);
}


// variables utilizadas en la rutina

//como cambiamos la frec del contador ahora 1 seg=100

volatile uint8_t count_seg= 0;
volatile uint16_t segundos=0;

uint8_t display =1; // con esta variable config cuales display estan encendidos
                    // 001 D1 ; 010 D2 ; 100 D3
                    //para ir alternando primero lo desplazaremos para generar los otros digitos y luego lo negaremos de este modo 001->110
uint8_t unidades = 0;
uint8_t decenas = 0;
uint8_t centenas = 0;

//Creamos nuestra rutina de interrupcion
ISR (TIMER1_COMPA_vect){
	
	count_seg++;
	if (count_seg==100) {                 //paso un seg
		segundos++;
		count_seg=0;
		
		if (segundos==1000) {
			segundos=0;
		}
		
		// Descomponemos el numero
		unidades = segundos%10;
		decenas = (segundos%100)/10;
		centenas = segundos/100;
		
		
			
	}
	
	display=  display<<1 ;
	if (display==8)
	{
		display=1;
	}
	PORTC = display;
	
	//para cambiar de display vamos desplazandolo 
	// MOSTRAMOS EL VALOR NUMÉRICO en cada display segun corresponda
	switch (display)
	{
		case D1:
		display_num(unidades);
		break;
		
		case D2:
		display_num(decenas);
		break;
		
		case D3:
		display_num(centenas);
		break;
	}
}
