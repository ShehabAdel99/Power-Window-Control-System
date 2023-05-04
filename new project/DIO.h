#ifndef dioh
#define dioh

#include "tm4c123gh6pm.h"
#include "types.h"
#include "bitwise_operation.h"
#define delay(n) for(unsigned long i=0; i < n; ++i)

void DIO_init(void);
void DIO_WritePort(v_uint64_ptr port,uint64 value);
void DIO_WritePin(v_uint64_ptr port,uint8 pin, uint8 value);
void DIO_PORTE_init();
void DIO_PORTC_init();
void DIO_PORTB_init();
void DIO_PORTA_init(void);
void keypad_Init(void);
char keypad_getkey(void);
int8 read_pin(uint64 port, int8 pin);
uint32 read_port(uint64 port);
void toggle_pin(v_uint64_ptr port,uint8 pin);

#endif

