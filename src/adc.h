#ifndef BLDC_CONTROLLER_ADC_H
#define BLDC_CONTROLLER_ADC_H

#include <avr/io.h>
#include <stdint.h>

#define ADC_SOURCE_PIN_PA2 ADCA.CH0
#define ADC_SOURCE_PIN_PA3 ADCA.CH1
#define ADC_SOURCE_PIN_PA4 ADCA.CH2
#define ADC_SOURCE_PIN_PA5 ADCA.CH3

#define AC_SOURCE_PIN_PA3 AC_MUXPOS_PIN3_gc
#define AC_SOURCE_PIN_PA4 AC_MUXPOS_PIN4_gc
#define AC_SOURCE_PIN_PA5 AC_MUXPOS_PIN5_gc

#define AC_SOURCE_PIN_VIRTUAL_GROUND AC_MUXNEG_PIN7_gc

#define _ADC_CAT(a, b) a##b

void adc_configure();

void ac_configure();

#define adc_read_sample(p, res) \
do { \
    /* Remove interrupt flag. */ \
    p.INTFLAGS = 0x01; \
    /* Start conversion. */ \
    p.CTRL |= ADC_CH_START_bm; \
    /* Wait for result */ \
    while (!p.INTFLAGS); \
    /* Return result. */ \
    res = _ADC_CAT(p, RES); \
} while (0)

#define adc_read_two_samples(p1, p2, res1, res2) \
do { \
    /* Remove interrupt flag. */ \
    p1.INTFLAGS = true; \
    p2.INTFLAGS = true; \
    /* Start conversion. */ \
    p1.CTRL |= ADC_CH_START_bm; \
    p2.CTRL |= ADC_CH_START_bm; \
    /* Wait for result */ \
    while (!p1.INTFLAGS); \
    res1 = _ADC_CAT(p1, RES); \
    while (!p2.INTFLAGS); \
    res2 = _ADC_CAT(p2, RES); \
} while (0)

#endif //BLDC_CONTROLLER_ADC_H
