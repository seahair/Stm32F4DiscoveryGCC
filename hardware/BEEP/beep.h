#ifndef  __BEEP_H__
#define  __BEEP_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


#define  BEEPPIN	GPIO_Pin_8
#define  BEEPON		1
#define  BEEPOFF	0

typedef struct _BEEP{
	u8		value;
	u16		hz;
	u32		time;		//time for beepon, ms

	void (* BeepInit) (void);	
	void (* BeepOn) (void);
	void (* BeepOff) (void);
	void (* BeepOnHzTime) (u16, u32);
	void (* PlayMusic) ( void );
}BEEP;

extern BEEP Beep;

void BeepInit( void );
void BeepOn( void );
void BeepOff( void );
void BeepOnHzTime( u16 hz, u32 time );
void PlayMusic( void );


#endif
