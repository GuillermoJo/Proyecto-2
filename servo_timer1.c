#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include "servo_timer1.h"

#define SERVO1_MIN_US 500U
#define SERVO1_MAX_US 2500U

void servo_timer1_init(void)
{
	// D9  = PB1 = OC1A
	// D10 = PB2 = OC1B
	DDRB |= (1 << PB1) | (1 << PB2);

	TCCR1A = 0;
	TCCR1B = 0;

	// Fast PWM modo 14, TOP = ICR1
	TCCR1A |= (1 << WGM11);
	TCCR1B |= (1 << WGM12) | (1 << WGM13);

	// Salida no invertida en OC1A y OC1B
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

	// Prescaler = 8
	TCCR1B |= (1 << CS11);

	// 20 ms
	// 16 MHz / 8 = 2 MHz
	// 1 cuenta = 0.5 us
	// 20 ms = 20000 us = 40000 cuentas
	ICR1 = 39999;

	servo_timer1_set_us(0, 1500);
	servo_timer1_set_us(1, 1500);
}

void servo_timer1_set_us(uint8_t canal, uint16_t pulso_us)
{
	if (pulso_us < SERVO1_MIN_US)
	{
		pulso_us = SERVO1_MIN_US;
	}

	if (pulso_us > SERVO1_MAX_US)
	{
		pulso_us = SERVO1_MAX_US;
	}

	// 1 cuenta = 0.5 us
	uint16_t cuentas = pulso_us * 2;

	if (canal == 0)
	{
		OCR1A = cuentas;   // D9
	}
	else if (canal == 1)
	{
		OCR1B = cuentas;   // D10
	}
}