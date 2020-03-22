#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>

#include "config.h"
#include "clock.h"
#include "timer.h"
#include "motor.h"
#include "uart.h"
#include "adc.h"

void setup() {
    clock_configure();
    uart_configure();
    adc_configure();
    ac_configure();
    timer_configure();
    motor_configure();

    // Disable configuration change protection for 4 cycles.
    sei();
    CCP = CCP_IOREG_gc;
    PMIC.CTRL = PMIC_LOLVLEN_bm | PMIC_MEDLVLEN_bm | PMIC_HILVLEN_bm;
    PMIC.INTPRI = 0x00;

    PORTE.DIRSET = PIN0_bm | PIN1_bm;
    PORTA.DIRCLR = PIN3_bm | PIN4_bm | PIN5_bm | PIN7_bm;

    uart_write_char('\0');
    uart_write_char('\1');
    uart_write_char('\2');
    uart_write_char('\3');
}

#define OFF_TIME (50)

#define _PHASE_BEMF_A ADC_SOURCE_PIN_PA3
#define _PHASE_BEMF_B ADC_SOURCE_PIN_PA4
#define _PHASE_BEMF_C ADC_SOURCE_PIN_PA5
#define _PHASE_BEMF(p) _PHASE_BEMF_ ## p

#define _PHASE_BEMF_PRINTF_A printf("%d;0;0\n", b)
#define _PHASE_BEMF_PRINTF_B printf("0;%d;0\n", b)
#define _PHASE_BEMF_PRINTF_C printf("0;0;%d\n", b)
#define _PHASE_BEMF_PRINTF(p) _PHASE_BEMF_PRINTF_ ## p

#define BEMF_EDGE_RISING 1
#define BEMF_EDGE_FALLING 2

ISR(ACA_AC0_vect) {
    PORTE.OUTTGL = PIN1_bm;
}

#define PHASE(high, f, low, bemf_edge) \
do { \
    int16_t s = 0; \
    int16_t b = 0; \
    int16_t target = 3000; \
    /* uint8_t initial = 0; */ \
    uint8_t i = 0; \
    for (i = 0; i < 10; i++) { \
        MOTOR_PORT.OUT = MOTOR_EN | (MOTOR_INH ## high | MOTOR_INL ## low); \
        _delay_us(1); \
        do { \
            adc_read_two_samples(ADC_SOURCE_PIN_PA2, _PHASE_BEMF(f), s, b); \
            /* adc_read_sample(ADC_SOURCE_PIN_PA2, s); */ \
        } while (s > target); \
        MOTOR_PORT.OUT = MOTOR_EN | (MOTOR_INL ## high | MOTOR_INL ## low); \
        /* printf("%d\n", b); */ \
        if (bemf_edge == BEMF_EDGE_FALLING && b < 570) { break; } \
        if (bemf_edge == BEMF_EDGE_RISING && b > 540) { break; } \
        _delay_us(OFF_TIME); \
    } \
    _delay_us(OFF_TIME); \
} while (0)


uint16_t pulse_count_reduce_counter = 1000;

void loop() {
//    pulse_count_reduce_counter--;
//    if (pulse_count_reduce_counter <= 1 && max_pulses > 5) {

//        pulse_count_reduce_counter = 1000;
//        if (max_pulses <= 5) { max_pulses = 5; }
//    }

    uint16_t max_pulses = 10000;

    while (true) {
        max_pulses--;
        for (int j = 0; j < 10000; ++j) {
            PHASE(A, B, C, BEMF_EDGE_FALLING);
            PHASE(B, A, C, BEMF_EDGE_RISING);
            PHASE(B, C, A, BEMF_EDGE_FALLING);
            PHASE(C, B, A, BEMF_EDGE_RISING);
            PHASE(C, A, B, BEMF_EDGE_FALLING);
            PHASE(A, C, B, BEMF_EDGE_RISING);
        }
    }



//    uint16_t a = 0;
//    adc_read_sample(_PHASE_BEMF(A), a);
//    PORTE.OUTSET = PIN1_bm;
//    printf("%d;0;0\n", a);
//    PORTE.OUTCLR = PIN1_bm;

//
//    uint16_t a = 0;
//    uint16_t b = 0;
//    uint16_t c = 0;
//    adc_read_sample(_PHASE_BEMF(A), a);
//    adc_read_sample(_PHASE_BEMF(B), b);
//    adc_read_sample(_PHASE_BEMF(C), c);
//    sum = a + b + c;
//    sum = a;
//    printf("%d;%d;%d\n", a, b, c);
//    printf("%d;%d;%d\n", a, b, c);
//    uart_write_char('\0');
//    uart_write_char('\0');
//    uart_write_int16(a);
//    uart_write_int16(b);
//    uart_write_int16(c);
}

int main(void) {
    setup();
    while (1) {
        loop();
    }
    return 0;
}
