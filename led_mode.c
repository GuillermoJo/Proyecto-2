#include <avr/io.h>
#include "led_mode.h"

// Usamos D13

void led_init(void)
{
	DDRB |= (1 << PB5);
}

void led_manual(void)
{
	PORTB |= (1 << PB5);
}

void led_eeprom(void)
{
	PORTB &= ~(1 << PB5);
}

void led_uart(void)
{
	PORTB ^= (1 << PB5);
}