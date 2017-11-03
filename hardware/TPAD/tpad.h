#ifndef  __TPAD_H__
#define  __TPAD_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#define  TPADPIN        GPIO_Pin_5




//void TpadPinInitIO( void );
//void TpadPinPullDown( void );
//void TpadPinInitCap( void );
//void TpadTimeInit1us( u32 per );
//void TpadInterruptInit( void );
void TpadInit( void );
void TpadClear( void );
void TpadInterruptStart( void );
void TpadInterruptStop( void );
u32  TpadGetCapTime( void );





#endif
