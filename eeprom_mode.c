#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdint.h>

#include "eeprom_mode.h"
#include "servo_timer1.h"
#include "servo_timer2.h"

#define FRAME_SIZE 8   // 4 servos × 2 bytes

// -----------------------------------------------------
// Retardo variable en ms
// -----------------------------------------------------
static void delay_variable_ms(uint16_t ms)
{
	while (ms > 0)
	{
		_delay_ms(1);
		ms--;
	}
}

// -----------------------------------------------------
// Guardar frame en EEPROM
// -----------------------------------------------------
void eeprom_guardar_frame(uint8_t index,
uint16_t s0,
uint16_t s1,
uint16_t s2,
uint16_t s3)
{
	uint16_t base;

	if (index >= MAX_FRAMES)
	return;

	base = index * FRAME_SIZE;

	eeprom_update_word((uint16_t*)(base + 0), s0);
	eeprom_update_word((uint16_t*)(base + 2), s1);
	eeprom_update_word((uint16_t*)(base + 4), s2);
	eeprom_update_word((uint16_t*)(base + 6), s3);
}

// -----------------------------------------------------
// Leer frame desde EEPROM
// -----------------------------------------------------
void eeprom_leer_frame(uint8_t index,
uint16_t *s0,
uint16_t *s1,
uint16_t *s2,
uint16_t *s3)
{
	uint16_t base;

	if (index >= MAX_FRAMES)
	return;

	base = index * FRAME_SIZE;

	*s0 = eeprom_read_word((uint16_t*)(base + 0));
	*s1 = eeprom_read_word((uint16_t*)(base + 2));
	*s2 = eeprom_read_word((uint16_t*)(base + 4));
	*s3 = eeprom_read_word((uint16_t*)(base + 6));
}

// -----------------------------------------------------
// Reproducir secuencia guardada
// -----------------------------------------------------
void eeprom_reproducir(uint8_t total_frames, uint16_t delay_ms)
{
	uint16_t s0, s1, s2, s3;

	if (total_frames > MAX_FRAMES)
	total_frames = MAX_FRAMES;

	for (uint8_t i = 0; i < total_frames; i++)
	{
		eeprom_leer_frame(i, &s0, &s1, &s2, &s3);

		// Servos Timer1
		servo_timer1_set_us(0, s0);
		servo_timer1_set_us(1, s1);

		// Servos Timer2
		servo_timer2_set(0, (uint8_t)s2);
		servo_timer2_set(1, (uint8_t)s3);

		// Esperar antes del siguiente frame
		delay_variable_ms(delay_ms);
	}
}