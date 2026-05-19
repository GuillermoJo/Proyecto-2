#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(uint16_t ubrr);
void uart_tx(char c);
char uart_rx(void);
void uart_print(char *txt);

#endif