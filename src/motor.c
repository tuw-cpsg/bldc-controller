#include "motor.h"

#include <avr/io.h>
#include "config.h"

void motor_configure() {
    // Set Pin for INHA to out.
    MOTOR_PORT.DIR |= MOTOR_INHA;
    // Set Pin for INLA to out.
    MOTOR_PORT.DIR |= MOTOR_INLA;

    // Set Pin for INHB to out.
    MOTOR_PORT.DIR |= MOTOR_INHB;
    // Set Pin for INLA to out.
    MOTOR_PORT.DIR |= MOTOR_INLB;

    // Set Pin for INHC to out.
    MOTOR_PORT.DIR |= MOTOR_INHC;
    // Set Pin for INLC to out.
    MOTOR_PORT.DIR |= MOTOR_INLC;

    // Set Pin for EN to out.
    MOTOR_PORT.DIR |= MOTOR_EN;
    // Set Pin for FAULT to in.
    MOTOR_PORT.DIR |= MOTOR_FAULT;

    // Set motor status pins to inputs.
    MOTOR_STATUS_PORT.DIRCLR = MOTOR_STATUS_SOx | MOTOR_STATUS_PHASE_A | MOTOR_STATUS_PHASE_B | MOTOR_STATUS_PHASE_C;

    motor_off();
}
