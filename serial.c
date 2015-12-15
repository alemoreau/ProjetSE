#define F_CPU 16000000UL       
#define BAUD 1000000
#define UBRR ((F_CPU)/(BAUD*16UL)-1)

#define SIZE 1024

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
volatile int count = 0;

void uart_transmit (unsigned char data); 
void uart_init (void);
void timer_init(void);

// FIFO IMPLEMENTATION
typedef struct {
  char * buf;
  int head;
  int tail;
  int size;
} fifo_t;

static fifo_t * uart_fifo;
 
//This initializes the FIFO structure with the given buffer and size
void fifo_init(fifo_t * f, char * buf, int size){
  f->head = 0;
  f->tail = 0;
  f->size = size;
  f->buf = buf;
}
 
//This reads nbytes bytes from the FIFO
//The number of bytes read is returned
int fifo_read(fifo_t * f, void * buf, int nbytes){
  int i;
  char * p;
  p = buf;
  for(i=0; i < nbytes; i++){
    if( f->tail != f->head ){ //see if any data is available
      *p++ = f->buf[f->tail];  //grab a byte from the buffer
      f->tail++;  //increment the tail
      if( f->tail == f->size )  //check for wrap-around
			f->tail = 0;
    } 
    else 
      return i; //number of bytes read 
  }
  return nbytes;
}
 
//This writes up to nbytes bytes to the FIFO
//If the head runs in to the tail, not all bytes are written
//The number of bytes written is returned
int fifo_write(fifo_t * f, const void * buf, int nbytes){
  int i;
  const char * p;
  p = buf;
  for(i=0; i < nbytes; i++){
    //first check to see if there is space in the buffer
    if( (f->head + 1 == f->tail) ||
      ( (f->head + 1 == f->size) && (f->tail == 0) ) )
      return i; //no more room
    else {
      f->buf[f->head] = *p++;
      f->head++;  //increment the head
      if( f->head == f->size )  //check for wrap-around
		f->head = 0; 
    }
  }
  return nbytes;
}

void uart_init (void)
{
  UBRR0H = (UBRR>>8);
  UBRR0L = UBRR;
  UCSR0A &= ~(_BV(FE0) | _BV(DOR0) | _BV(UPE0));
  UCSR0A |= _BV(U2X0);
  UCSR0B =(1<<TXEN0)|(1<<RXEN0);
  UCSR0C =(3<<UCSZ00);
}

ISR(INT0_vect)
{
  char data = 0;
  int value = (PIND & (1<<PD4)) ? 1 :0;                                                                                                               
  int clk = (PIND & (1<<PD2)) ? 1 :0;
  
  data = (clk << 1);                                                                                                                                 
  data |= value;
  fifo_write(uart_fifo, &data, 1);
  count++;
}


void uart_transmit (unsigned char data)
{
  while (!( UCSR0A & (1<<UDRE0)));
  UDR0 = data;
  // load data in the register
}
void uart_transmit_fifo()
{
  char * data;
  fifo_read(uart_fifo, data, 1);
  uart_transmit(*data);
}


int main() {
  count = 0;
  char buf[SIZE];
  fifo_t fifo;
  uart_fifo = &fifo;
  uart_init();                                                  

  fifo_init(uart_fifo, buf, SIZE);

  
  DDRD = ~((_BV(PD2))|(_BV(PD4)));                                                                                                                  
  PORTD = ~((0 << PORTD2)|(1 << PORTD4));

  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
  EIMSK |= (1 << INT0);  	// enable interrupt on INT0

    sei();          // enable global interrupts

	while(1)
	{
		if (count != 0)
		{
			uart_transmit_fifo();
			count--;
		}
	}
	return 0;
}