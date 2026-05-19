#include <avr/io.h>
#include "uart.h"

void uart_init(uint16_t ubrr)
{
	UBRR0H = (ubrr >> 8);
	UBRR0L = ubrr;

	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

char uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uart_print(char *txt)
{
	while (*txt)
	{
		uart_tx(*txt++);
	}
}