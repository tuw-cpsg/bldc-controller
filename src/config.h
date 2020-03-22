#ifndef BLDC_CONTROLLER_CONFIG_H_
#define BLDC_CONTROLLER_CONFIG_H_

#define MOTOR_PORT PORTC
#define MOTOR_INHA (1 << 0)
#define MOTOR_INLA (1 << 1)
#define MOTOR_INHB (1 << 2)
#define MOTOR_INLB (1 << 3)
#define MOTOR_INHC (1 << 4)
#define MOTOR_INLC (1 << 5)
#define MOTOR_EN (1 << 6)
#define MOTOR_FAULT (1 << 7)

#define MOTOR_STATUS_PORT PORTA
#define MOTOR_STATUS_VDD (1 << 1)
#define MOTOR_STATUS_SOx (1 << 2)
#define MOTOR_STATUS_PHASE_A (1 << 3)
#define MOTOR_STATUS_PHASE_B (1 << 4)
#define MOTOR_STATUS_PHASE_C (1 << 5)

#endif // BLDC_CONTROLLER_CONFIG_H_
