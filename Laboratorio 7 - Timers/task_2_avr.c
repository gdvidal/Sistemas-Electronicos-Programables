#include <avr/io.h>
#define F_CPU 16000000UL

#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

int led = 0;
int presionado = 0;
int presionado_mc=0;
int count_debounce = 0;
int count_button = 0;
int debounce_checks = 100;

float contador_tiempo = 0;

volatile int bol=0;

volatile int unidad = 0;
volatile int primer_dec = 0;
volatile int seg_dec = 0;
volatile int ter_dec = 0;

volatile int digito = 0;

void init_timer0(void)
{
	//TCCR0A: OC0A BEHAVIOR
	OCR0A = 250;				//prescale 64: periodo de 1ms
	TCCR0A |= (1 << WGM01);		//ctc
	TCCR0A |= (1 << COM0A1);  //clear oc0a on compare match

	TCCR0B |= (1 << CS01) | (1<<CS00); //define el clock
	TIMSK0 |= (1 << OCIE0A);
}

void init_timer0B(void)
{
	OCR0B = 156;
	TCCR0A |= (1 << WGM01);		//ctc periodo: 0.28ms
	TCCR0A |= (1 << COM0B1);

	TCCR0B |= (1 << CS01) | (1<<CS00);//prescale 64
	TIMSK0 |= (1 << OCIE0B); //enmascara interrupcion para ocr0b

}


void init_timer2(void)
{
	OCR2A = 156;
	TCCR2A |= (1 << WGM21);		//ctc
	TCCR2A |= (1 << COM2A1);

	TCCR2B |= (1 << CS22);  //64 prescaler
	TIMSK2 |= (1 << OCIE2A);
}

void init_button(void)
{
	DDRC &= ~(1 << DDC0);			// PC0 como entrada
	PORTC |= (1 << 0);				//PULL-UP RESISTOR
	PCICR |= (1 << PCIE1);          //HABILITA INTERRUPCION PARA PUERTOS C
	PCMSK1 |= (1 << PCINT8);       //PCINT12 ASOCIADO A PC4, ENMASCARA LA INTERRUPCION
}

void init_button_mc(void)
{
	DDRB &= ~(1 << DDB7);			// PB7 como entrada
	PORTB |= (1 << 7);				//PULL-UP RESISTOR
	PCMSK0 |= (1 << PCINT7); 
	PCICR |= (1 << PCIE0);          //HABILITA INTERRUPCION PARA PUERTOS C
	
	
}

void prender_led(void)
{
	DDRB |= (0b00001110);
	PORTB |=(1<<1); //rojo
	PORTB |=(1<<2); //verde
	
	//PORTB |=(1<<3); //azul
	PORTC |= (1<<5);

	if (led == 3)
	{
		led = 0;
	}
	led++;

	switch(led){

		case (0):		
		break;

		case(3):		//VERDE
		PORTB &= ~(1<<2);
		PORTB |= (1<<1);
		//PORTB |= (1<<3);
		PORTC |= (1<<5);
		break;

		case(2):		//ROJO
		PORTB &=~(1<<1);
		PORTB |=(1<<2);
		//PORTB |=(1<<3);
		PORTC |= (1<<5);
		break;

		case(1):		//AZUL
		PORTC &=~ (1<<5);
		PORTB |=(1<<2);
		PORTB |=(1<<1);
		break;
	}

}

void prender_numero(int n)
{
	switch(n)
	{
		case 0:
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB &=~ (1<<PORTB5); //F
		PORTB |= (1<<PORTB4);  //G
		PORTD |= (1<<PORTD7); //PUNTO
		break;

		case 1:
		//PORTD = 0b11111001;
		PORTD |= (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB |= (1<<PORTB4);
		PORTD |= (1<<PORTD7);
		
		break;

		case 2:
		//PORTD = 0b10100100;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD |= (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 3:
		//PORTD = 0b10110000;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 4:
		//PORTD = 0b10011001;
		PORTD |=(1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB &~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 5:
		//PORTD = 0b10010010;
		PORTD &=~ (1<<PORTD0);
		PORTD |= (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB &=~(1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 6:
		//PORTD = 0b1000010;
		PORTD &=~ (1<<PORTD0);
		PORTD |= (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB &=~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 7:
		//PORTD = 0b11111000;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB |=(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 8:
		PORTD &=~ (1<<PORTD0);
		PORTD &=~(1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB &=~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;

		case 9:;
		//PORTD = 0b10011000;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB &=~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD |= (1<<PORTD7);
		break;
	}


}

void prender_numero_punto(int n)
{
	switch(n)
	{
		case 0:
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB &=~ (1<<PORTB5); //F
		PORTB |= (1<<PORTB4);  //G
		PORTD &=~ (1<<PORTD7); //PUNTO
		break;

		case 1:
		//PORTD = 0b11111001;
		PORTD |= (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB |= (1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		
		break;

		case 2:
		//PORTD = 0b10100100;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD |= (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 3:
		//PORTD = 0b10110000;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 4:
		//PORTD = 0b10011001;
		PORTD |=(1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB &~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 5:
		//PORTD = 0b10010010;
		PORTD &=~ (1<<PORTD0);
		PORTD |= (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB &=~(1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 6:
		//PORTD = 0b1000010;
		PORTD &=~ (1<<PORTD0);
		PORTD |= (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB &=~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 7:
		//PORTD = 0b11111000;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB |= (1<<PORTB5);
		PORTB |=(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 8:
		PORTD &=~ (1<<PORTD0);
		PORTD &=~(1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD &=~ (1<<PORTD3);
		PORTD &=~ (1<<PORTD4);
		PORTB &=~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;

		case 9:;
		//PORTD = 0b10011000;
		PORTD &=~ (1<<PORTD0);
		PORTD &=~ (1<<PORTD1);
		PORTD &=~ (1<<PORTD2);
		PORTD |= (1<<PORTD3);
		PORTD |= (1<<PORTD4);
		PORTB &=~ (1<<PORTB5);
		PORTB &=~(1<<PORTB4);
		PORTD &=~ (1<<PORTD7);
		break;
	}


}


int main(void)
{
	DDRD |= 0b11111111;  //todos los PDn como configurados como salidas
	DDRC |= 0b00111110;  // pin PC3, PC2, PC1 : DIGITOS DEL DISPLAY, PC5=AZUL

	DDRB |= (1<<DDB4); //F
	DDRB |= (1<<DDB5); //G


	init_timer0();
	init_timer0B();
	init_timer2();
	
	init_button();
	init_button_mc();
	
	sei();

	while (1);
}

ISR (TIMER0_COMPA_vect)
{
	if (count_button)
	{
		count_debounce++;
	}

	else
	{
		count_debounce = 0;
	}

	if (count_debounce >= debounce_checks)
	{
		if (((PINC & (1 << 0)) == 0) && (presionado == 1))
		{
			prender_led();


		}
		count_debounce = 0;
		count_button = 0;
		PCICR |= (1 << PCIE1);
	}
}

ISR (PCINT1_vect)  //PCINT1 = PUERTO C
{
	presionado ^= (1 << 0);
	count_button = 1;
	PCICR &= ~(1 << PCIE1);
}

ISR (PCINT0_vect) //PCINT0 = PUERTO B
{
	presionado_mc ^= (1 << 0);
	
	if (presionado_mc==1)
	{
		unidad=0;
		primer_dec=0;
		seg_dec=0;
		ter_dec=0;
		PORTC &=~ (1<<4);
	}
	
	//PCICR &= ~(1 << PCIE0);*/
	//PORTC ^= (1<<4);
}


ISR (TIMER0_COMPB_vect)
{
	if (((PINB & (1 << 7)) == 0) && (presionado_mc == 1))
	{

		if (ter_dec ==9)
		{
			if (seg_dec==9)
			{
				if (primer_dec==9)
				{
					if (unidad==9)
					{
						//errror
						PORTC |= (1<<4);
						
					}

					else
					{
						unidad++;
						primer_dec=0;
						seg_dec=0;
						ter_dec=0;

					}

				}

				else
				{
					primer_dec++;
					seg_dec=0;
					ter_dec=0;
				}

			}

			else
			{
				seg_dec++;
				ter_dec=0;
			}

		}

		else
		{
			ter_dec++;

		}


	}

}


ISR (TIMER2_COMPA_vect)
{

	digito++;

	if (digito == 4)
	{
		digito = 1;
	}

	//// ////// NUMERO DECIMAL

	if (unidad < 1)
	{
		if (digito == 1)
		{
			//PORTC |= 0b00001000;
			//PORTC &= ~(0b00000110);
			PORTC |= (1<<PORTC3);
			PORTC &=~ (1<<PORTC2);
			PORTC &=~ (1<<PORTC1);
			prender_numero_punto(primer_dec);
		}
		else if (digito == 2)
		{
			//PORTC |= 0b00000100;
			//PORTC &= ~(0b00001010);
			PORTC |= (1<<PORTC2);
			PORTC &=~ (1<<PORTC3);
			PORTC &=~ (1<<PORTC1);
			prender_numero(seg_dec);
		}
		else if (digito == 3)
		{
			//PORTC |= 0b00000010;
			//PORTC &= ~(0b00001100);
			PORTC |= (1<<PORTC1);
			PORTC &=~ (1<<PORTC2);
			PORTC &=~ (1<<PORTC3);
			prender_numero(ter_dec);
		}
	}

	//// /////// NUMERO>1

	else if (unidad >= 1)
	{
		if (digito == 1)
		{
			//PORTC |= 0b00001000;
			//PORTC &= ~(0b00000110);
			PORTC |= (1<<PORTC3);
			PORTC &=~ (1<<PORTC2);
			PORTC &=~ (1<<PORTC1);
			prender_numero(seg_dec);
		}
		else if (digito == 2)
		{
			//PORTC |= 0b00000100;
			//PORTC &= ~(0b00001010);
			PORTC |= (1<<PORTC2);
			PORTC &=~ (1<<PORTC3);
			PORTC &=~ (1<<PORTC1);
			prender_numero(primer_dec);
		}
		else if (digito == 3)
		{
			//PORTC |= 0b00000010;
			//PORTC &= ~(0b00001100);
			PORTC |= (1<<PORTC1);
			PORTC &=~ (1<<PORTC2);
			PORTC &=~ (1<<PORTC3);
			prender_numero_punto(unidad);
		}
	}


}