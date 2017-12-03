/*
 * TestLjud.c
 *
 * Created: 2017-06-10 16:37:56
 * Author : joel_
 */

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BAUDRATE 9600
#define BAUD_PRESCALLER (((F_CPU / (BAUDRATE * 16UL))) - 1)

//USART
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);

int main(void)
{
	USART_init();

  DDRD |= 1 << PIND2;
	// Turn on ADC
	ADCSRA |= 1 << ADEN;
	// 50kHZ - 200kHZ
	//Enable Prescaler 5 - 20 (10bits) else higher.
	ADCSRA |= 1 << ADPS2;
	//Using AVCC as ref VOLTAGE
	ADMUX  |= 1 << REFS0;
	//choosing storage type
	ADMUX |= 1 << ADLAR;
	sei();
	//Enable interrupt
	ADCSRA |= 1 << ADIE;
	//Start first conversion
	ADCSRA |= 1 << ADSC;


    while (1)
    {
    }
}

ISR(ADC_vect){
	int soundValue;
	soundValue = ADCH;
	if(soundValue > 75){
		PORTD |= 1 << PIND2;
		USART_send('1');
		_delay_ms(100);
		PORTD &= ~(1<< PIND2);
	}
	ADCSRA |=  1<< ADSC;
}


void USART_init(void){

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_receive(void){

	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;

}

void USART_send( unsigned char data){

	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = data;

}

void USART_putstring(char* StringPtr){

	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
		StringPtr++;
		}

}
