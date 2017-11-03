#ifndef  __LED_H__
#define  __LED_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


//void LED_Init( void );


//#define LedRed    GPIO_Pin_9
//#define LedGreen  GPIO_Pin_10
#define LEDRED    GPIO_Pin_9
#define LEDGREEN  GPIO_Pin_10


#define LedStatus_ON   0
#define LedStatus_OFF  1

typedef struct _LED LED;




#if 1	
typedef struct _LED{
	u16  pin;
	void (*LedInit) ( LED* );
	void (*LedON) ( LED* );
	void (*LedOFF) ( LED* );
	void (*SetValue) ( LED*, u8 );
    u8   (*GetLedStatus) ( LED* );
	void (*LedBlink) ( LED*, u16 );
	void (*LedRollBack) ( LED* );
	
}LED;
#endif	

extern LED LedRed;
extern LED LedGreen;
//void LED_Init_G( void );



void LedInit( LED* led );
void LedOn( LED* led );
void LedOff( LED* led );
void SetValue( LED* led, u8 value );
u8 GetLedStatus( LED* led );
void LedBlink( LED*led, u16 time );
void LedRollBack( LED* led );


/*
void LedInitRed( void );
void LedOnRed( void );
void LedOffRed( void );
void SetValueLedRed( u8 value );
u8 GetLedStatusRed( void );
void LedBlinkRed( u16 time );
void LedRollBackRed( void );

void LedInitGreen( void );
void LedOnGreen( void );
void LedOffGreen( void );
void SetValueLedGreen( u8 value );
u8 GetLedStatusGreen( void );
void LedBlinkGreen( u16 time );
void LedRollBackGreen( void );
*/

/*
void LedInit( u16 pin );
void LedON( u16 pin );
void LedOFF( u16 pin );
void LedBlink( u16 pin );
u8   GetLedStatus( u16 pin );
void LedRollBack( u16 pin );
*/


#endif
