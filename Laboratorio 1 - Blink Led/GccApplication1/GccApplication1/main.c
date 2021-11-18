/*
 * GccApplication1.c
 *
 * Created: 25-08-2019 2:02:09
 * Author : Guillermo
 */ 

#define F_CPU 16000000UL // define frecuencia a la que trabajará el mc,
//esta definicion sirve para hacer calculos en algunas librerias,
//como en la librería delay.h, que requiere saber la frecuencia del micro
// para poder calcular los tiempos de espera de sus rutinas.

#include <avr/io.h> //contiene todas las definiciones y declaraciones de los registros internos y
//perifericos del micro, con las cuales podemos configurar todos los perifericos
//del micro.


#include <util/delay.h> //contiene las funciones y macros para realizar retardos del orden de
//microsegundos y milsegundos.



int main(void)
{
	
	DDRB |= (1<< DDB3); // config del pin como salida
	PORTB |= (1<<PORTB3); // estado del pin. nivel de voltaje alto
	
	DDRB |= (0<< DDB7); // config como entrada e interruptor
	
	
	while (1)
	{
		if (PINB & (1<<DDB7)){ // lectura del pin pb7: si NO esta apretado:
			
			PORTB |= (1<<PORTB3);
			_delay_ms(500);
			PORTB &= ~ (1<<PORTB3);
			_delay_ms(500);
		}
		
		else{
			PORTB |= (1<<PORTB3);
			_delay_ms(1500);
			PORTB &= ~ (1<<PORTB3);
			_delay_ms(1500);
			
				
		}
	}
}

