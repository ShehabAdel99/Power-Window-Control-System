/* This program controls the direction and speed of DC motor using PWM and L298N Motor driver */
#include "motor.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "types.h"
#include "bitwise_operation.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#define PWM_FREQUENCY 50
#define PWM_MAX 100

void Motor_PORTA_init(void)
{
  SYSCTL_RCGCGPIO_R|=0x01;
  while((SYSCTL_PRGPIO_R&0x01)!=0x01);
  GPIO_PORTA_LOCK_R=0X4C4F434B;
  GPIO_PORTA_CR_R|=0x0F;
  GPIO_PORTA_DIR_R=0x06;
  GPIO_PORTA_DEN_R=0x06;
}

void PWM_init(void)
{
    // Enable clock to PWM1 and GPIO Port A
    SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R1;
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

    // Configure PA6 as PWM1 output
    GPIO_PORTA_AFSEL_R |= 0x40;
    GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA6_M1PWM2;
    GPIO_PORTA_DEN_R |= 0x40;

    // Configure generator 1, sub-timer A of PWM1
    PWM1_1_CTL_R = 0x00000000;
    PWM1_1_GENA_R = PWM_1_GENA_ACTCMPAD_ZERO | PWM_1_GENA_ACTLOAD_ONE;
    PWM1_1_LOAD_R = (80000000 / (PWM_MAX * PWM_FREQUENCY)) - 1;
    PWM1_1_CMPA_R = 0;
    PWM1_1_CTL_R |= PWM_1_CTL_ENABLE;

    // Configure output 2 of generator 1
    PWM0_ENABLE_R   |=0x04;        
}

void delay_ms(int time_ms)
{
    uint32_t i, j;
    for (i = 0; i < time_ms; i++)
    {
        for (j = 0; j < 3180; j++)
        {
            // Wait for 1 ms using NOP
            __asm("NOP");
        }
    }
}

void motor_forward(void)
{
    // Set PWM duty cycle to 75%
   // PWM1_1_CMPA_R = (PWM_MAX * 75) / 100;

    // Set direction by driving IN1 and IN2 signals
    GPIO_PORTA_DATA_R |= 0x02;
    GPIO_PORTA_DATA_R &= ~0x04;
}

void motor_backward(void)
{
    // Set PWM duty cycle to 50%
  //  PWM1_1_CMPA_R = (PWM_MAX * 50) / 100;

    // Set direction by driving IN1 and IN2 signals
    GPIO_PORTA_DATA_R &= ~0x02;
    GPIO_PORTA_DATA_R |= 0x04;
}
