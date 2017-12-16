#ifndef  __TOUCH_H__
#define  __TOUCH_H__

#include "stm32f4xx.h"



typedef struct _TOUCH_ATTR{
	u16  x;
	u16  y;
	u8 dir;
}TOUCH_ATTR;

typedef struct _TOUCH_DIR{
	s8 (*init) ( void );
	s8 (*match) ( void );
	void (*getxy) ( u16 *x, u16 *y );
}TOUCH_DIR;

s8 TouchInit( void );
s8 TouchMatch( void );
void TouchGetxy( u16 *x, u16 *y );

#endif
