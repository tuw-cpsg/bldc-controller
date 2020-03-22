#include "adc.h"

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stddef.h>

static uint8_t read_calibration_byte(uint8_t index) {
    uint8_t result;
    NVM_CMD = NVM_CMD_READ_CALIB_ROW_gc;
    result = pgm_read_byte(index);
    NVM_CMD = NVM_CMD_NO_OPERATION_gc;
    return result;
}

/* VREF = 1.65V
 * dV = VREF * 0.05 = 0.0825
 * RES = (VINP - (-dV)) / VREF * (TOP + 1)
 */
void adc_configure() {
    // Use PA0 (AREF) as input.
    PORTA.DIRCLR = PIN0_bm;
    // Load calibration data.
    ADCA.CALL = read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0)); // First read is junk. Ignore it.
    ADCA.CALH = read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1)); // First read is junk. Ignore it.
    ADCA.CALL = read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL0));
    ADCA.CALH = read_calibration_byte(offsetof(NVM_PROD_SIGNATURES_t, ADCACAL1));
    // Use 2MHz clock for ClkADC.
    ADCA.PRESCALER |= ADC_PRESCALER_DIV8_gc;
    // Use external reference for ADC on AREFA.
//    ADCA.REFCTRL = ADC_REFSEL_INTVCC_gc;
    ADCA.REFCTRL = ADC_REFSEL_AREFA_gc;
//    ADCA.REFCTRL = ADC_REFSEL_INTVCC2_gc;
//    ADCA.REFCTRL = ADC_REFSEL_INT1V_gc;
    // Use 12 bit resolution. (1.4us per sample in 12 Bit mode, 1.2us per sample in 8 Bit mode)
    ADCA.CTRLB |= ADC_RESOLUTION_12BIT_gc;
    // Set all channels to single ended mode
    ADCA.CH0.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH1.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH2.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
    ADCA.CH3.CTRL |= ADC_CH_INPUTMODE_SINGLEENDED_gc;
    // Connect each pin to the corresponding channel.
    ADCA.CH0.MUXCTRL = ADC_CH_MUXPOS_PIN2_gc;
    ADCA.CH1.MUXCTRL = ADC_CH_MUXPOS_PIN3_gc;
    ADCA.CH2.MUXCTRL = ADC_CH_MUXPOS_PIN4_gc;
    ADCA.CH3.MUXCTRL = ADC_CH_MUXPOS_PIN5_gc;
    // Clear interrupt flag.
    ADCA.INTFLAGS = 0;
    // Enable ADC.
    ADCA.CTRLA |= AC_ENABLE_bm;
}

void ac_configure() {
//    ACA.AC0MUXCTRL = AC_SOURCE_PIN_VIRTUAL_GROUND | AC_SOURCE_PIN_PA3;
//    ACA.AC0CTRL = AC_HSMODE_bm | AC_HYSMODE_LARGE_gc | AC_INTMODE_BOTHEDGES_gc | AC_INTLVL_HI_gc | AC_ENABLE_bm;
}