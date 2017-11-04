#ifndef  __TPAD_H__
#define  __TPAD_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#define  TPADPIN			GPIO_Pin_5
#define  TPADHZ				84 
#define  TPADPERIOD			0XFFFFFFFF 
#define  TPADSTUSSTART		0X00
#define  TPADSTUSWAIT		0X01
#define  TPADSTUSFINISH		0X02
#define  TPADTHRESHOLD      100



//extern u32 TPADTime = 0;


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
u32  TpadTestDefaultTime( u8 num );  //num < 10




#endif
