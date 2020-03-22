#include "timer.h"

#include <avr/io.h>

void timer_configure() {
    // Set timer prescaler to Clk/256 = increment every 8us.
    TCC0.CTRLA = TC_CLKSEL_DIV256_gc;
    // Set mode to normal.
    TCC0.CTRLB |= TC_WGMODE_NORMAL_gc;
    // Trigger medium priority interrupt on overflow.
    TCC0.INTCTRLA = TC_OVFINTLVL_MED_gc;
    // Initialize with 0.
    TCC0.CNT = 0;
    // Set period to 125. 8us*125 = 1ms.
    TCC0.PER = 125;
}
