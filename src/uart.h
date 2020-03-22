#ifndef BLDC_CONTROLLER_UART_H_
#define BLDC_CONTROLLER_UART_H_

#include <avr/io.h>
#include <stddef.h>

void uart_configure();

static inline void uart_write_char(char c) {
    while (!(USARTE0.STATUS & USART_DREIF_bm));
    USARTE0.DATA = c;
}

static inline void uart_write_string(char *str) {
    while (1) {
        uart_write_char(*str);
        if (*str == '\0') {
            break;
        }
        str++;
    }
}

static inline void uart_write_int16(int16_t v) {
    char* c = (char *) &v;
    uart_write_char(c[0]);
    uart_write_char(c[1]);
}

#endif // BLDC_CONTROLLER_UART_H_
