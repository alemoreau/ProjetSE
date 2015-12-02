#define F_CPU 16000000UL       
#define BAUD 250000
#define UBRR ((F_CPU)/(BAUD*16UL)-1)

#define SIZE 1024

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile char current_time = 0;
volatile char current_state = 0;
const unsigned char MASK_STATE = 128; // 1000000
const unsigned char MASK_TIME = 127;  // 0111111


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
  UBRR0H=(UBRR>>8);
  UBRR0L=UBRR;
  UCSR0A &= ~(_BV(FE0) | _BV(DOR0) | _BV(UPE0));
  UCSR0B|=(1<<TXEN0)|(1<<RXEN0);
  UCSR0C|=(1<<USBS0)|(3<<UCSZ00); 
}

void timer_init(void)
{
  // initialize Timer0
  cli();          // disable global interrupts

  TCCR1A = 0;     // set entire TCCR1A register to 0
  TCCR1B = 0;     // same for TCCR1B

  // set compare match register to desired timer count:
  OCR1A = 15624; //16000000 / ((1024) * (15624+1)) = 1Hz
  //OCR1A = 124 ; //16000000 /(124+1) = 128 kHz

  // turn on CTC mode:
  TCCR1B |= (1 << WGM12);

  // No prescaler
  TCCR1B |= (1 << CS10);
  TCCR1B |= (1 << CS12); //1024 prescaler

  // enable timer compare interrupt:
  TIMSK1 |= (1 << OCIE1A);


  //  DDRD &= ~(1 << DDD2);     // Clear the PD2 pin
  // PD2 (PCINT0 pin) is now an input

  //PORTD |= (1 << PORTD2);    // turn On the Pull-up
  // PD2 is now an input with pull-up enabled



  EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
  EIMSK |= (1 << INT0);     // Turns on INT0


  sei();          // enable global interrupts
}

ISR (INT0_vect)
{
  /*current_state = ~current_state;
  char message = (current_time & MASK_TIME) | (current_state & MASK_STATE) ;
  char i;
  //uart_transmit(message);
  uart_transmit('b');
  for (i = 0; i < 8 ; ++i)
      uart_transmit('0'+ ((message >> (7-i)) & 1));
  uart_transmit('\n');
  uart_transmit('\r');*/
  char i;
  char data = 0;                                                                                                                             
  int value = (PIND & (1<<PD4)) ? 1 :0;                                                                                                               
  int clk = (PIND & (1<<PD2)) ? 1 :0;                                                                                                                
  char msg[3];
                                                                                                                                                     
  data = (clk << 1);                                                                                                                                 
  data |= value;   

	msg[0] = data;
	msg[1] = '\n';
	msg[2] = '\r';

  fifo_write(uart_fifo, msg, 3);
  
  /*uart_transmit(data); 
  for (i = 0; i < 8 ; ++i)
    uart_transmit('0'+ ((data >> (7-i)) & 1));
  
  uart_transmit('\n');                        
  uart_transmit('\n');                        
  uart_transmit('\r');  

  */
  /*
  for (i = 0; i < 8; ++i)
    msg[i] = '0'+ ((data >> (7-i)) & 1);
  msg[8] = '\n';
  msg[9] = '\r';
  fifo_write(uart_fifo, msg, 10);
  */
}

ISR(TIMER1_COMPA_vect)
{
  if (!(current_time++ & MASK_TIME)){
    char i;
    char message = current_state & MASK_STATE;
    //uart_transmit(message);
    char msg[10];
    //uart_transmit('a');
    //for (i = 0; i < 8; ++i)
    //  uart_transmit('0'+ ((message >> (7-i)) & 1));
    //uart_transmit('\n');
    //uart_transmit('\r');

    for (i = 0; i < 8; ++i)
      msg[i] = '0'+ ((message >> (7-i)) & 1);
    msg[8] = '\n';
    msg[9] = '\r';
    fifo_write(uart_fifo, msg, 10);
  }
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
  char buf[SIZE];
  fifo_t fifo;
  uart_fifo = &fifo;
  
  uart_init();                                                  
  timer_init();
  fifo_init(uart_fifo, buf, SIZE);

  DDRD = ~((_BV(PD2))|(_BV(PD4)));                                                                                                                  
  PORTD = ~((0 << PORTD2)|(1 << PORTD4));

	while(1)
	{
		uart_transmit(uart_fifo->tail);
		uart_transmit('\n');
		uart_transmit('\r');
		
		uart_transmit(uart_fifo->head);
		uart_transmit('\n');
		uart_transmit('\n');
		uart_transmit('\r');
		
		_delay_ms(1000);
		
		
		if (uart_fifo->head < uart_fifo->tail )
			uart_transmit_fifo();
	}
  return 0;

}




