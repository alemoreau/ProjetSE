#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU 16000000UL
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1


char buffer;
volatile char charbuff;
char bufferPos;

void USART_Init( unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 2stop bit */
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSR0A & (1<<UDRE0)) )
;
/* Put data into buffer, sends the data */
UDR0 = data;
}

int main(void)
{
	
	USART_Init(MYUBRR);
	
	DDRB = _BV(PB1);
	
	DDRD = ~((_BV(PD2))|(_BV(PD4)));
    PORTD = ~((1 << PORTD2)|(1 << PORTD4));    // turn off the Pull-up  

    EICRA = ((1 << ISC01)|(0 << ISC00));    // set INT0 to trigger on ANY logic change
    EIMSK = (1 << INT0);     // Turns on INT0
	PORTB = _BV(PB1);
	
	
	buffer = 0;
	charbuff = 0;
	bufferPos = 0;
	
	
    sei();                    // turn on interrupts
    while(1)
    {
		USART_Transmit(charbuff);
		_delay_ms(1);
    }
}

ISR(INT0_vect)
{	
	int value = (PIND & (1<<PD4)) ? 1 :0;
	
	
	if(bufferPos > 0 && bufferPos < 9) {
		buffer |= (value << (bufferPos-2));
    }
	
	bufferPos++;
	
	if(bufferPos == 11)
	{
		PORTB = ~PORTB;
		charbuff = buffer;
		bufferPos =0;
		buffer = 0;
		_delay_ms(250);
	}
}