#ifndef SERVO_TIMER1_H
#define SERVO_TIMER1_H

#include <stdint.h>

void servo_timer1_init(void);
void servo_timer1_set_us(uint8_t canal, uint16_t pulso_us);

#endif