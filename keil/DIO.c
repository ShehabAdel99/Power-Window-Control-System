#include <stdint.h>
#include "DIO.h"
#include "tm4c123gh6pm.h"
#include "types.h"
#include "bitwise_operation.h"


void DIO_PORTC_init()
{
  SYSCTL_RCGCGPIO_R|=0x4; 	
  while((SYSCTL_PRGPIO_R&0x4)!=0x4);
  GPIO_PORTC_LOCK_R=0X4C4F434B;
  GPIO_PORTC_CR_R|=0xF0;
  GPIO_PORTC_DIR_R=0xF0;
  GPIO_PORTC_DEN_R=0xF0;
   for (int i=4;i<=7;i++)
  {
    //Set_Bit(GPIO_PORTC_ODR_R,i);
    //Set_Bit(GPIO_PORTC_PUR_R,i);
  }
  
}

void DIO_PORTE_init()
{
  SYSCTL_RCGCGPIO_R|=0x10;
  while((SYSCTL_PRGPIO_R&0x10)!=0x10);
  GPIO_PORTE_LOCK_R=0X4C4F434B;
  GPIO_PORTE_CR_R|=0x0F;
  GPIO_PORTE_DIR_R=0x00;
  GPIO_PORTE_DEN_R=0x0F;
  /*for (int i=0;i<=3;i++)
  {
    //Set_Bit(GPIO_PORTE_ODR_R,i);
    Set_Bit(GPIO_PORTE_PUR_R,i);
  }
	*/

  GPIO_PORTE_PUR_R=0x0F;
}
void DIO_PORTF_init()
{
  SYSCTL_RCGCGPIO_R|=0x20;
  while((SYSCTL_PRGPIO_R&0x20)!=0x20);
  GPIO_PORTF_LOCK_R=0X4C4F434B;
  GPIO_PORTF_CR_R|=0x1F;
  GPIO_PORTF_DIR_R=0x0E;
  GPIO_PORTF_DEN_R=0x1F;
  /*for (int i=0;i<=3;i++)
  {
    //Set_Bit(GPIO_PORTE_ODR_R,i);
    Set_Bit(GPIO_PORTE_PUR_R,i);
  }
	*/

  GPIO_PORTF_PUR_R=0x11;
}

void DIO_PORTB_init()
{
  SYSCTL_RCGCGPIO_R|=0x2;
  while((SYSCTL_PRGPIO_R&0x2)!=0x2);
  GPIO_PORTB_DEN_R|=0xFF;
  GPIO_PORTB_DIR_R|=0xFF;
  
  
}

void DIO_init(void)
{
  SYSCTL_RCGCGPIO_R|=0x20; 
  while((SYSCTL_PRGPIO_R&0x20)!=0x20);
  GPIO_PORTF_LOCK_R=0X4C4F434B;
  GPIO_PORTF_CR_R|=0x1F;
  GPIO_PORTF_DIR_R=0x0E;
  GPIO_PORTF_PUR_R=0x11;
  GPIO_PORTF_DEN_R=0x1F;
  
}

void DIO_WritePort(v_uint64_ptr port,uint64 value)
{
  *port=value;
}

void DIO_WritePin(v_uint64_ptr port,uint8 pin, uint8 value)
{
  if(value==0)
  {
    Clear_Bit(*port,pin);
  }
  else if (value==1)
  {
    Set_Bit(*port,pin);
  }
 
}
int8 read_pin(uint64 port,int8 pin)
{
  return Get_Bit(port,pin);
  
}
uint32 read_port(uint64 port)
{
  return port;
  
}
void toggle_pin(v_uint64_ptr port,uint8 pin)
{
  Toggle_Bit(*port,pin);
  
}
