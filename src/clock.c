#include "clock.h"

#include <avr/io.h>

void clock_configure() {
    // Enable internal 32MHz oscillator.
    OSC.CTRL |= OSC_RC32MEN_bm;
    // Wait until oscillator is stable.
    do {} while ((OSC.STATUS & OSC_RC32MRDY_bm) == 0);
    // Disable configuration change protection for 4 cycles.
    CCP = CCP_IOREG_gc;
    // Use the 32MHz oscillator for ClkSYS.
    CLK.CTRL = CLK_SCLKSEL_RC32M_gc;
    // ClkSYS = ClkPER4 = ClkPER2 = ClkCPU = ClkPER.
    CLK.PSCTRL = CLK_PSADIV_1_gc | CLK_PSBCDIV_1_1_gc;
}
