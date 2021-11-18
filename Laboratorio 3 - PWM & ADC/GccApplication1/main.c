/*
 * GccApplication1.c
 *
 * Created: 08-09-2019 15:28:45
 * Author : Guillermo
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <avr/interrupt.h>


void pwm_init(){
	
	//// PWM_0 /////
	
	//modo fast pwm sin limite
	TCCR0A |= (1<<WGM00) ;
	TCCR0A |=(1<<WGM01) ;
	
	//clock prescaler: n=8
	
	TCCR0B &=~(1<<CS00); 
	TCCR0B |=(1<<CS01); 
	TCCR0B &=~(1<<CS02); 
		
		
	///// PWM_2 ////////
	
	//modo fast pwm sin limite
	TCCR2A |= (1<<WGM20) ;
	TCCR2A |=(1<<WGM21) ;
	
	//clock prescaler: n=8
	
	TCCR2B &=~(1<<CS20); 
	TCCR2B |=(1<<CS21); 
	TCCR2B &=~(1<<CS22); 
	
}

//ROJO
void setDuty_channel1 (float  duty){
	//OCR0 como salida
	DDRD |= (1<< DDD6);
	
	//señal no invertida
	TCCR0A |= (1<<COM0A1);
	
	//luminosidad(duty)
	OCR0A= 255.0f - (255.0f * (duty/100)) ;
	
}

//AZUL
void setDuty_channel2 (float duty){
	
	DDRB |= (1<< DDB3);
	//señal no invertida
	TCCR2A |= (1<<COM2A1);
	
	//luminosidad
	OCR2A= 255.0f - (255.0f * (duty/100)) ;
	

}
//VERDE
void setDuty_channel3 (float duty){
	
	DDRD |= (1<< DDD3);
	
	//señal no invertida
	TCCR2A |= (1<<COM2B1);
	
	//luminosidad
	OCR2B= 255.0f - (255.0f * (duty/100)) ;
		
}


//////////////////////////

void ADC_init()
{
	// Output adjust = right // bits se alinean a la derecha
	
	ADMUX &=~ (1<<ADLAR);

	// Voltage Reference = AVCC with external capacitor at ARef//
	ADMUX |=  (1<<REFS0); // 5V
	ADMUX &=~ (1<<REFS1);

	// Frequency divisor = 128 -> 16000/128 = 125 KHz
	ADCSRA |= (1<<ADPS0);
	ADCSRA |= (1<<ADPS1);
	ADCSRA |= (1<<ADPS2);
}

int ADC_GetData(int canal)
{
	// Selecci?n del canal de lADC // ADMUX = 0 -> ADC0
	ADMUX &=~  0x0F;
	ADMUX |=  canal;
	//DEBEMOS SELECCIONAR EL CANAL 0.
	
	// Encendemos en ADC
	ADCSRA |= (1<<ADEN);
	_delay_us(10);	// Esperamos a que caliente

	// Mandamos el muestreo, single conversion mode
	ADCSRA |= (1<<ADSC);

	// Esperamos a que muestree, leyendo el flag
	while( !(ADCSRA & (1<<ADIF)) );
	ADCSRA |= (1<<ADIF);	// Reiniciamos el flag

	// Apagamos el ADC
	ADCSRA &=~ (1<<ADEN);

	return ADC;
}


void Convert_chanel1(int state)
{
	if (state==1){
		float adcV= ADC_GetData(0)*5.0f/1024.0f;
		
		if (adcV>0.0f && adcV<0.7f){
			setDuty_channel1(2);
		}
		
		else if (adcV >= 0.7f && adcV< 1.0f)
		{
			setDuty_channel1(5);
			
		}
		
		else if (adcV >= 1.0f && adcV< 2.0f)
		{
			setDuty_channel1(25);
			
		}
		
		else if (adcV >= 2.0f && adcV< 3.0f)
		{
			setDuty_channel1(50);
			
		}
		
		else if (adcV >= 3.0f && adcV< 4.0f)
		{
			setDuty_channel1(75);
			
		}
		
		else if (adcV >= 4.0f && adcV< 5.0f)
		{
			setDuty_channel1(100);
			
		}
				
	}
	
	
		
}

void Convert_chanel2(int state2)
{
	if (state2==1){
		float adcV= ADC_GetData(0)*5.0f/1024.0f;
		
		if (adcV>0.0f && adcV<0.7f){
			setDuty_channel2(2);
		}
		
		else if (adcV >= 0.7f && adcV< 1.0f)
		{
			setDuty_channel2(5);
			
		}
		
		else if (adcV >= 1.0f && adcV< 2.0f)
		{
			setDuty_channel2(25);
			
		}
		
		else if (adcV >= 2.0f && adcV< 3.0f)
		{
			setDuty_channel2(50);
			
		}
		
		else if (adcV >= 3.0f && adcV< 4.0f)
		{
			setDuty_channel2(75);
			
		}
		
		else if (adcV >= 4.0f && adcV< 5.0f)
		{
			setDuty_channel2(100);
			
		}
		
		
	} 
	
	
	
}

void Convert_chanel3(int state3)
{
	if (state3==1){
		float adcV= ADC_GetData(0)*5.0f/1024.0f;
		
		if (adcV>0.0f && adcV<0.7f){
			setDuty_channel3(2);
		}
		
		else if (adcV >= 0.7f && adcV< 1.0f)
		{
			setDuty_channel3(5);
			
		}
		
		else if (adcV >= 1.0f && adcV< 2.0f)
		{
			setDuty_channel3(25);
			
		}
		
		else if (adcV >= 2.0f && adcV< 3.0f)
		{
			setDuty_channel3(50);
			
		}
		
		else if (adcV >= 3.0f && adcV< 4.0f)
		{
			setDuty_channel3(75);
			
		}
		
		else if (adcV >= 4.0f && adcV< 5.0f)
		{
			setDuty_channel3(100);
			
		}
		
	}
	
	
}

volatile int mood=-1;

int main(void){
	
	pwm_init();
	ADC_init();
	
	DDRB |= (0<< DDB7); //boton pb7 como entrada
	DDRD |= (1<< DDD0);
	cli();	//deshabilitamos interrupciones globales
	
	//configuracion de interrupciones
	
	PCICR |= (1<<PCIE0); //Habilita las interrupciones por grupo (B)
	PCMSK0 |= (1<<PCINT7); //Restringimos la interrupcion
	
	sei();
	
	while(1){
		
		
		if (mood==1){
			Convert_chanel2(0);
			Convert_chanel3(0);
			Convert_chanel1(1);
			
		}
		
		else if (mood==2){
			Convert_chanel1(0);
			Convert_chanel3(0);
			Convert_chanel2(1);
			
		}
		
		else if (mood==3){
			Convert_chanel2(0);
			Convert_chanel1(0);
			Convert_chanel3(1);
		}
		
		/*
		switch (mood){
		case 1:
		
		Convert_chanel1(1);
		Convert_chanel2(0);
		Convert_chanel3(0);
		break;
		
		case 2:
		PORTD ^=(1<<DDD0);
		Convert_chanel1(0);
		Convert_chanel2(1);
		Convert_chanel3(0);
		break;
		
		case 3:
		Convert_chanel1(0);
		Convert_chanel2(0);
		Convert_chanel3(1);
		break;
	}*/
		
	}
	
}


// rutina de interrupcion 


ISR (PCINT0_vect){
	mood+=1;
	
	if (mood>3){
		mood=1;
	}
}

