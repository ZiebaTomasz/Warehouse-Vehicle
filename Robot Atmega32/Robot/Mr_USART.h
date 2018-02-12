#ifndef MrUSART
#define MrUSART
#define F_CPU 16000000UL
#include <util/delay.h>
//#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE  (((F_CPU / (BAUD * 16UL))) - 1)




#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/atomic.h>
#include <avr/delay.h>
#include <string.h>
//#include <avr/pgmspace.h>
#include <avr/sleep.h>
#include <util/atomic.h>

volatile unsigned char data_in[10];
volatile unsigned char command_in[10];
#define CHAR_NEWLINE '\n'
#define CHAR_RETURN '\r'
#define RETURN_NEWLINE "\r\n"

void USART_init()
{
	
	UBRRH = (BAUD_PRESCALE >> 8);
	UBRRL = BAUD_PRESCALE;

    UCSRB = (1<<RXCIE)|(1<<RXEN)|(1<<TXEN);                 //aktywne przerwanie od odbioru oraz zmiana trybu dzia³ania pinów D0 i D1
	UCSRC = (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);


}



void usart_putString (const char *data)
{
	// Cycle through each character individually
	while (*data) {
		USART_transmitByte(*data++);
	}
}



void USART_transmitByte(char data) {
	/* Wait for empty transmit buffer */
while ((UCSRA & (1 << UDRE)) == 0) {};
UDR = data;                                         /* send data */
}

uint8_t USART_receiveByte(void) {
	loop_until_bit_is_set(UCSRA, RXC);       /* Wait for incoming data */
	return UDR;                                /* return register value */
}


void print_value (char *id, int *value)
{
	char buffer[8];
	itoa(value, buffer, 10);
	USART_transmitByte(id);
	USART_transmitByte(':');
	usart_putString(buffer);
	usart_putString(RETURN_NEWLINE);
}



/* Here are a bunch of useful printing commands */

void USART_printString(const char myString[]) {
	uint8_t i = 0;
	while (myString[i]) {
		USART_transmitByte(myString[i]);
		i++;
	}
}

void USART_readString(volatile char myString[], volatile uint8_t maxLength) {
	char response;
	uint8_t i;
	i = 0;
	while (i < (maxLength - 1)) {                   /* prevent over-runs */
		response = USART_receiveByte();
		USART_transmitByte(response);                                    /* echo */
		if (response == '\r') {                     /* enter marks the end */
			break;
		}
		else {
			myString[i] = response;                       /* add in a letter */
			i++;
		}
	}
	myString[i] = 0;                          /* terminal NULL character */
}

unsigned long parse_assignment ()
{
	char *pch;
	char cmdValue[16];
	// Find the position the equals sign is
	// in the string, keep a pointer to it
	pch = strchr(command_in, '=');
	// Copy everything after that point into
	// the buffer variable
	strcpy(cmdValue, pch+1);
	// Now turn this value into an integer and
	// return it to the caller.
	return atoi(cmdValue);
}
void copy_command ()
{
	// The USART might interrupt this - don't let that happen!
	ATOMIC_BLOCK(ATOMIC_FORCEON) {
		// Copy the contents of data_in into command_in
		memcpy(command_in, data_in, 8);

		// Now clear data_in, the USART can reuse it now
		memset(data_in[0], 0, 8);
	}
}
	

	#endif