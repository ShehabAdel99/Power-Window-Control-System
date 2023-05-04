#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "TM4C123GH6PM.h"
#include "app.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOSConfig.h"
#include "DIO.h"
#include "types.h"
#include "bitwise_operation.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "hw_gpio.h"
#include "queue.h"
#include "semphr.h"
#include "motor.h"

void Delay_ms(int time_ms);
int duty_cycle = 4999;
void PWM_init(void);
void Turn_OtherDirection(void);
void Turn_oneDirection(void);



int main()
{
	
   // PWM_init();
Motor_PORTA_init();
    while (1)
    {
        motor_forward();
      //  delay_ms(1000);
     //   motor_backward();
       // delay_ms(1000);
    }
		
	
	return 0;
}




