
#define FOSC 16000000
#define BAUD 1000000
#define MYUBRR FOSC/16/BAUD-1
#define Connection_led PB7
#define Transmission_led PB4

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

bool captured = true;


void setconfigPins(){
  DDRB = _BV(Connection_led) | _BV(Transmission_led);
  PORTB = 0;
}
void USART_init(unsigned int ubrr){
  // set baudrate
  UBRR0H = (unsigned char)(ubrr>>8);
  UBRR0L = (unsigned char)(ubrr);

  // set TX RX
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ01)|_BV(UCSZ00);

}

void USART_transmit(uint8_t data){

  while (!(UCSR0A & _BV(UDRE0))) // wait for empty transmit buffer
  ;
  UDR0 = data; // put data to buffer, send data
}

void USART_packageTransmit(uint16_t data){
  uint8_t partsH = (uint8_t)((data & 0xFF00) >> 8); 
  uint8_t partsL = (uint8_t)(data & 0x00FF);

  USART_transmit(partsL);
  USART_transmit(partsH);
  
}

void USART_recive(char data){

  while(!(UCSR0A & _BV(RXC0)))
    ; // wait
  return UDR0; // receive
}

void initTimer4(){
  cli();

  TCCR4A = 0;
  TCCR4B = 0;
  TCCR4C = 0;

  TCNT4 = 0;
  TIMSK4 = 0;

  // TIFR4 |= _BV(ICF4) |  _BV(TOV4); 
  TCCR4B |= _BV(ICES4) | _BV(CS40);
  // Enable input capture and overflow interrupts
  TIMSK4 |= _BV(ICIE4) | _BV(TOIE4); 

  sei();
}

ISR(TIMER4_OVF_vect){

}
ISR(TIMER4_CAPT_vect){
        
 USART_packageTransmit(ICR4);
}

int main(void){
  PORTB |= _BV(Connection_led);
  PORTL |= _BV(PL0);
  USART_init(MYUBRR);
  initTimer4();
 
  while(1)
    {
    }
}