#ifndef __LED_H
#define __LED_H	 

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "tick.h"

#define Red 0x01
#define Green 0x02
#define Blue 0x04
void LED_Init(void);//≥ı ºªØ
void turnOffLed2(void);
void turnOffLed1(void);
void turnOnLed2Red(void);
void turnOnLed2Green(void);
void turnOnLed2Blue(void);
void tunnOnLed1(void);
void led2ShowRed(void);
void led2ShowGreen(void);	
void led2ShowBlue(void);
void led2blink(char color,int n,int gab_ms);
		 				    
#endif
