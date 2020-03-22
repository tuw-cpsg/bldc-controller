#ifndef BLDC_CONTROLLER_MOTOR_H
#define BLDC_CONTROLLER_MOTOR_H

#include "adc.h"
#include "config.h"

void motor_configure();

#define motor_off() \
do { \
    MOTOR_PORT.OUT = MOTOR_EN | MOTOR_INLA | MOTOR_INLB | MOTOR_INLC; \
} while(0)

#endif //BLDC_CONTROLLER_MOTOR_H
