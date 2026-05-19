#define F_CPU 16000000UL

#include <avr/io.h>
#include <stdint.h>
#include "servo_timer2.h"

#define SERVO2_MIN     16U
#define SERVO2_CENTRO  24U
#define SERVO2_MAX     31U

void servo_timer2_init(void)
{
	// D11 = PB3 = OC2A
	// D3  = PD3 = OC2B
	DDRB |= (1 << PB3);
	DDRD |= (1 << PD3);

	TCCR2A = 0;
	TCCR2B = 0;

	// Fast PWM, TOP = 255
	TCCR2A |= (1 << WGM21) | (1 << WGM20);

	// Salida no invertida en OC2A y OC2B
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);

	// Prescaler = 1024
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

	servo_timer2_set(0, SERVO2_CENTRO);
	servo_timer2_set(1, SERVO2_CENTRO);
}

void servo_timer2_set(uint8_t canal, uint8_t cuentas)
{
	if (cuentas < SERVO2_MIN)
	{
		cuentas = SERVO2_MIN;
	}

	if (cuentas > SERVO2_MAX)
	{
		cuentas = SERVO2_MAX;
	}

	if (canal == 0)
	{
		OCR2A = cuentas;   // D11
	}
	else if (canal == 1)
	{
		OCR2B = cuentas;   // D3
	}
}