#include "uart.h"

#include <avr/io.h>
#include <stdio.h>

static int _uart_putchar(char c, FILE *stream) {
    if (c == '\n')
        _uart_putchar('\r', stream);
    while (!(USARTE0.STATUS & USART_DREIF_bm));
    USARTE0.DATA = c;
    return 0;
}

static int _uart_getchar(FILE *stream) {
    while (!(USARTE0.STATUS & USART_RXCIF_bm));
    char data = USARTE0.DATA;
    if (data == '\r')
        data = '\n';
    return data;
}

static FILE _uart_stream = FDEV_SETUP_STREAM(_uart_putchar, _uart_getchar, _FDEV_SETUP_RW);

void uart_configure() {
    PORTE.REMAP |= PORT_USART0_bm;
    PORTE.DIRCLR = PIN2_bm;
    PORTE.DIRSET = PIN3_bm;

    // BSEL = 2094 and BSCALE = -7 should result in baud r ate of 115212.
//    uint16_t BSEL = 2094;
//    int8_t BSCALE = -7;
    uint16_t BSEL = 1;
    int8_t BSCALE = 0;
    // Use lower 8 bits of BSEL.
    USARTE0.BAUDCTRLA = (uint8_t) BSEL;
    // Use BSCALE for upper 4 bits and the upper 4 bits of BSEL for lower bits.
    USARTE0.BAUDCTRLB = (BSCALE << 4) | (uint8_t) (BSEL >> 8);


    // Disable interrupts, just for safety.
    USARTE0.CTRLA = 0;
    // 8 data bits, no parity and 1 stop bit.
    USARTE0.CTRLC |= USART_CHSIZE_8BIT_gc;

    //Enable receive and transmit with double transmission speed.
    USARTE0.CTRLB = USART_TXEN_bm | USART_RXEN_bm | USART_CLK2X_bm;

    stdout = stdin = &_uart_stream;
}
