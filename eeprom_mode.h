#ifndef EEPROM_MODE_H
#define EEPROM_MODE_H

#include <stdint.h>

#define MAX_FRAMES 20

void eeprom_guardar_frame(uint8_t index,
uint16_t s0,
uint16_t s1,
uint16_t s2,
uint16_t s3);

void eeprom_leer_frame(uint8_t index,
uint16_t *s0,
uint16_t *s1,
uint16_t *s2,
uint16_t *s3);

void eeprom_reproducir(uint8_t total_frames, uint16_t delay_ms);

#endif