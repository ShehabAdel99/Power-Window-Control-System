#include <stdint.h>
#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include "TM4C123GH6PM.h"
#include "macros.h"
#include "types.h"
#include "FreeRTOSConfig.h"
#include "bitwise_operation.h"
#include "DIO.h"
#define PortA_IRQn 0


xSemaphoreHandle xBinarySemaphore;
xSemaphoreHandle xMutex;
xQueueHandle xQueue;


uint8_t jamming = 0;
void sensorButtonInit(void);
void motorInit(void);
void limitInit(void);
void buttonsInit(void);
void lockButtonInit(void);
void jamTask(void* pvParameters);
void recieveQueue(void* pvParameters);
void driver(void* pvParameters);
void passenger(void* pvParameters);
void vApplicationIdleHook();
void GPIOA_Handler(void);

// Idle Task
void vApplicationIdleHook(){
while(1)
{
	
	delay(100);
}
}

// Jamming Task
void jamTask(void* pvParameters) {

    while (1) {
        //TAKE SEMAPHORE
        xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
        // MOVE DOWN FOR 0.5 SECOND
			for(int i =1 ; i<500000;i++)
			{
					GPIO_PORTF_DATA_R &= ~(1 << 3);
					GPIO_PORTF_DATA_R |= (1 << 2);
			}

		
				// Stop motor
			  GPIO_PORTF_DATA_R &= ~(1 << 3);
        GPIO_PORTF_DATA_R &= ~(1 << 2);

				jamming=1;
   }  
}

// Functions that makes the action
void recieveQueue(void* pvParameters) {
		uint8_t Val;
		portBASE_TYPE xStatus;
	  const portTickType xTicks=100/portTICK_RATE_MS;
	while(1)
	{
		xStatus=xQueueReceive(xQueue,&Val,xTicks);
	 if ( xStatus != pdFALSE ) 
		{
			if(Val==0) // STOP
			{
					GPIO_PORTF_DATA_R &= ~(1 << 3);
					GPIO_PORTF_DATA_R &= ~(1 << 2);
			}
			else if(Val==1) //MOVE UP
			{
					GPIO_PORTF_DATA_R |= (1 << 3);
					GPIO_PORTF_DATA_R &= ~(1 << 2);
			}
			else if(Val==2) //MOVE DOWN
			{
					GPIO_PORTF_DATA_R &= ~(1 << 3);
					GPIO_PORTF_DATA_R |= (1 << 2);
			}	
	
		Val=4;
	}
		else{
			// make red led toggles to detedt that there is no action happennig
			Toggle_Bit(GPIO_PORTF_DATA_R,1);
			delay(30);
		}
	}
	
}

void driver(void* pvParameters){
		uint8_t Val;
	  portBASE_TYPE xStatus;
		while(1)
		{
			// Applying Mutix
			xSemaphoreTake(xMutex,portMAX_DELAY );
			
//MANUAL UP			
			if (GET_BIT(GPIO_PORTD_DATA_R,0)==1){ 
				Val=1;
					while((GET_BIT(GPIO_PORTD_DATA_R,0)==1))
						{
							if ((GET_BIT(GPIO_PORTC_DATA_R,6)==1))
							{
							Val=0;	
							}	
											xStatus = xQueueSendToBack(xQueue,&Val,0);

						}

				Val=0;
				xStatus = xQueueSendToBack(xQueue,&Val,0);
			}
			
// AUTO UP
			if (GET_BIT(GPIO_PORTA_DATA_R,4)==1){ 
				Val=1;
					while((GET_BIT(GPIO_PORTC_DATA_R,6)==0)&&	jamming==0 && GET_BIT(GPIO_PORTA_DATA_R,5)==0)
						{	
											xStatus = xQueueSendToBack(xQueue,&Val,0);
						}
				jamming=0;
				Val=0;
				xStatus = xQueueSendToBack(xQueue,&Val,0);
			}
			
//MANUAL DOWN			
			if (GET_BIT(GPIO_PORTD_DATA_R,1)==1){
					Val=2;

					while((GET_BIT(GPIO_PORTD_DATA_R,1)==1))
						{
						  if ((GET_BIT(GPIO_PORTC_DATA_R,7)==1))
							{
							Val=0;	
							}	
											xStatus = xQueueSendToBack(xQueue,&Val,0);
							
						}
					Val=0;
					xStatus = xQueueSendToBack(xQueue,&Val,0);
				}
			
// AUTO DOWN
			if (GET_BIT(GPIO_PORTA_DATA_R,5)==1){ 
				Val=2;
					while((GET_BIT(GPIO_PORTC_DATA_R,7)==0) && GET_BIT(GPIO_PORTA_DATA_R,4)==0)
						{	
											xStatus = xQueueSendToBack(xQueue,&Val,0);
						}

				Val=0;
				xStatus = xQueueSendToBack(xQueue,&Val,0);
			}		

//CHECK LOCK			
			if (GET_BIT(GPIO_PORTF_DATA_R,4)==1){
					
					vTaskPrioritySet(NULL,2);
					
				}
				else
				{
					vTaskPrioritySet(NULL,1);
				}
				// Releasing Mutex
				xSemaphoreGive(xMutex);
				taskYIELD(); // checking Passenger
		}
	}

void passenger(void* pvParameters){
	uint8_t Val;
	portBASE_TYPE xStatus;
	while(1)
	{
		// Applying Mutex
		xSemaphoreTake(xMutex,portMAX_DELAY );

//MANUAL UP
		if (GET_BIT(GPIO_PORTD_DATA_R,2)==1){ 
			Val=1;
           while((GET_BIT(GPIO_PORTD_DATA_R,2)==1))
						{
							if ((GET_BIT(GPIO_PORTC_DATA_R,6)==1))
							{
							Val=0;	
							}							
											xStatus = xQueueSendToBack(xQueue,&Val,0);

						}     

						
			Val=0;
			xStatus = xQueueSendToBack(xQueue,&Val,0);
		}
// AUTO UP
			if (GET_BIT(GPIO_PORTA_DATA_R,2)==1){ 
				Val=1;
					while((GET_BIT(GPIO_PORTC_DATA_R,6)==0) && jamming==0 && GET_BIT(GPIO_PORTA_DATA_R,3)==0)
						{	
											xStatus = xQueueSendToBack(xQueue,&Val,0);
						}
				jamming=0;
				Val=0;
				xStatus = xQueueSendToBack(xQueue,&Val,0);
			}
		
//MANUAL DOWN		
		if (GET_BIT(GPIO_PORTD_DATA_R,3)==1){ 
  	 Val=2;
          while((GET_BIT(GPIO_PORTD_DATA_R,3)==1))
						{
							if ((GET_BIT(GPIO_PORTC_DATA_R,7)==1)){
							Val=0;	
							}
											xStatus = xQueueSendToBack(xQueue,&Val,0);
						}       

     Val=0;
		 xStatus = xQueueSendToBack(xQueue,&Val,0);
			
		}
// AUTO DOWN
			if (GET_BIT(GPIO_PORTA_DATA_R,3)==1){ 
				Val=2;
					while((GET_BIT(GPIO_PORTC_DATA_R,7)==0) && GET_BIT(GPIO_PORTA_DATA_R,2)==0)
						{	
											xStatus = xQueueSendToBack(xQueue,&Val,0);
						}

				Val=0;
				xStatus = xQueueSendToBack(xQueue,&Val,0);
			}			
		// Releasing Mutex
    xSemaphoreGive(xMutex);
		taskYIELD(); // Checking Driver
	}
	
	}

                         /*main function*/
/*------------------------------------------------------------------------*/
int main( void )
{
	
	
		DIO_PORTF_init();
	  xQueue = xQueueCreate(1,sizeof(uint8_t));
	  xMutex = xSemaphoreCreateMutex(); 
    sensorButtonInit();
	  lockButtonInit();
		buttonsInit();
		limitInit();
		motorInit();
		__ASM("CPSIE i");
		
	
	
	
		vSemaphoreCreateBinary(xBinarySemaphore);
		xBinarySemaphore = xSemaphoreCreateBinary();
	if( xBinarySemaphore != NULL )
		{
			// Tasks Creation
			
			xTaskCreate( jamTask, "jamTask", 200, NULL, 5, NULL );
			
			xTaskCreate( passenger, "passenger", 240, NULL, 1, NULL );
			
			xTaskCreate( driver, "driver", 240, NULL, 1, NULL );
			
			xTaskCreate( recieveQueue, "recieveQueue", 200, NULL, 3, NULL );
			
			/* Start the scheduler so the created tasks start executing. */
			vTaskStartScheduler();
		}

    /* If all is well we will never reach here as the scheduler will now be
    running the tasks.  If we do reach here then it is likely that there was
    insufficient heap memory available for a resource to be created. */
    for( ;; );
		return 0;
}


/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/

//Port-A handler
void GPIOA_Handler(void)
{
    //Clear uint8_terrupt Flag
    GPIO_PORTA_ICR_R |= (1<<6);
	
    portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    /* Clear the software uint8_terrupt bit using the uint8_terrupt controllers
    Clear Pending register. */
   // mainCLEAR_uint8_tERRUPT();
	  /* 'Give' the semaphore to unblock the task. */
    xSemaphoreGiveFromISR( xBinarySemaphore, &xHigherPriorityTaskWoken );
    portEND_SWITCHING_ISR( xHigherPriorityTaskWoken );
}

// Jamming Button init
void sensorButtonInit(void)
{
	
	    //Enable Port A
    SYSCTL_RCGCGPIO_R |= 0x01;
	
    //Configure Pin 6 in Port A as input
    GPIO_PORTA_DIR_R &= ~(1 << 6);
    GPIO_PORTA_CR_R |= (1 << 6);
    GPIO_PORTA_PUR_R |= (1 << 6);	
    GPIO_PORTA_DEN_R |= (1 << 6);
	
    //Enable uint8_terrupt on PORT A & set priority to 7
	  NVIC_PRI0_R |= (1<<7) | (1<<6) | (1<<5);
    NVIC_EN0_R |= (1<<0);
    
	
    //Configure uint8_terrupt on Pin 6 to detect FALLING edge
    GPIO_PORTA_IM_R &=0;
    GPIO_PORTA_IS_R &= ~(1<<6);
    GPIO_PORTA_IEV_R &= ~(1<<6);
    GPIO_PORTA_ICR_R |= (1<<6);
    GPIO_PORTA_IM_R |= (1<<6);	
	
}

// Lock Button init
void lockButtonInit(void)
{
	    //Enable Port F
    SYSCTL_RCGCGPIO_R |= 0x20;
	
    //Configure Pin 4 in Port E as input
	  GPIO_PORTA_LOCK_R=0X4C4F434B;
    GPIO_PORTF_DIR_R &= ~(1 << 4);
    GPIO_PORTF_CR_R |= (1 << 4);
    GPIO_PORTF_PUR_R |= (1 << 4);	
    GPIO_PORTF_DEN_R |= (1 << 4);
	
}

// Driver & Passenger Buttons
void buttonsInit(void)
{
	// Manual
	
		    //Enable Port D
    SYSCTL_RCGCGPIO_R |= 0x08;
	
    //Configure Pins (0,1)-> Driver ,  (2,3)-> Passenger  in Port D as input
    GPIO_PORTD_DIR_R &= ~((1 << 0)|(1<<1)|(1<<2)|(1<<3));
    GPIO_PORTD_CR_R |= (1 << 0)|(1<<1)|(1<<2)|(1<<3);
    GPIO_PORTD_PUR_R |= (1 << 0)|(1<<1)|(1<<2)|(1<<3);	
    GPIO_PORTD_DEN_R |= (1 << 0)|(1<<1)|(1<<2)|(1<<3);
	
	
	// Auto
	
			    //Enable Port A
    SYSCTL_RCGCGPIO_R |= 0x01;

	
    //Configure Pins (4,5)-> Driver ,  (2,3)-> Passenger  in Port A as input
    GPIO_PORTA_DIR_R &= ~((1<<2)|(1<<3)|(1 << 4)|(1<<5));
    GPIO_PORTA_CR_R |= (1<<2)|(1<<3)|(1 << 4)|(1<<5);
    GPIO_PORTA_PUR_R |= (1<<2)|(1<<3)|(1 << 4)|(1<<5);	
    GPIO_PORTA_DEN_R |= (1<<2)|(1<<3)|(1 << 4)|(1<<5);
	
	
	
}

// Limit Buttons init
void limitInit(void)
{
			    //Enable Port C
    SYSCTL_RCGCGPIO_R |= 0x04;

	
    //Configure Pins 6,7 in Port C as input ( 6 limit up , 7 limit down )
    GPIO_PORTC_DIR_R &= ~((1 << 6)|(1<<7));
    GPIO_PORTC_CR_R |= (1 << 6)|(1<<7);
    GPIO_PORTC_PUR_R |= (1 << 6)|(1<<7);	
    GPIO_PORTC_DEN_R |= (1 << 6)|(1<<7);
}

// Motor init
void motorInit(void)
{
	
	
			    //Enable Port F
    SYSCTL_RCGCGPIO_R |= 0x20;
		
    //Configure Pin 1,2,3 in Port F as input
    GPIO_PORTF_DIR_R |= ((1 << 2)|(1<<3));
    GPIO_PORTF_CR_R |= (1 << 2)|(1<<3);
    GPIO_PORTF_DEN_R |= (1 << 2)|(1<<3);



	
}