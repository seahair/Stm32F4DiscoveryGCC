#ifndef  __LED_H
#define  __LED_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


void LED_Init( void );


#define LedRed    GPIO_Pin_9
#define LedGreen  GPIO_Pin_10

#define LedStatus_ON   0
#define LedStatus_OFF  1

/*
struct LED{
	u16 pin;
	void (*LedInit) ( u16 pin);
	void (*LedON) ( void );
	void (*LedOFF) ( void );
	void (*LedBlink) ( void );
    u8   (*GetLedStatus) ( void );
	
};*/
	
void LedInit( u16 pin );
void LedON( u16 pin );
void LedOFF( u16 pin );
void LedBlink( u16 pin );
u8   GetLedStatus( u16 pin );
void LedRollBack( u16 pin );


#endif
