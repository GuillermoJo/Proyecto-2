#ifndef SERVO_TIMER2_H
#define SERVO_TIMER2_H

#include <stdint.h>

void servo_timer2_init(void);
void servo_timer2_set(uint8_t canal, uint8_t cuentas);

#endif