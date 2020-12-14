#ifndef _MYUART_H_
#define _MYUART_H_
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "buffer.h"

void myUartInit(void);
void Usart_SendByte(uint32_t moduleInstance, uint_fast8_t transmitData);
void Usart_SendString(uint32_t moduleInstance, char* str);
void myEUSCIA0Init(void);

#endif
