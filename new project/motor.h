#ifndef motor
#define motor
#include "TM4C123GH6PM.h"
#include "types.h"
#include "bitwise_operation.h"


#define PWM1_GENA_R         (*((volatile uint32_t *)0x40028040))
#define PWM1_LOAD_R         (*((volatile uint32_t *)0x40028008))
#define PWM1_CMPA_R         (*((volatile uint32_t *)0x40028018))
#define PWM1_CTL_ENABLE     1
#define PWM1_GENA_ACTCMPBD_M 0x00000C00

void PWM_init(void);
void delay_ms(int time_ms);
void motor_forward(void);
void motor_backward(void);
void Motor_PORTA_init(void);




#endif