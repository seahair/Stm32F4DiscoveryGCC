#ifndef  __TOUCH_H__
#define  __TOUCH_H__

#include "stm32f4xx.h"

#define  CT_MAX_TOUCH	0X05
#define  TOUCHDIVV		0X0
#define  TOUCHDIVH		0X1
#define  TOUCHSTAYES	0X1
#define  TOUCHSTANO		0X0

typedef struct _TOUCH_XY{
	u16 x[CT_MAX_TOUCH];
	u16 y[CT_MAX_TOUCH];
}TOUCH_XY;

typedef struct _TOUCH_ATTR{
	TOUCH_XY OriginalXY;	//触摸驱动芯片读出来的XY
	TOUCH_XY LCDXY;		//转换成显示屏上的坐标XY
	u8 dir;				//方向	 
	u8 sta;				//状态：0-无触摸，1-有触摸	
	u8 touchnum;		//按下的数量，小于CT_MAX_TOUCH	
}TOUCH_ATTR;

typedef struct _TOUCH_DIR{
	s8 (*init) ( void );
	s8 (*match) ( void );
	void (*getxy) ( TOUCH_ATTR *ptouchattr );
}TOUCH_DIR;

s8 TouchInit( void );
s8 TouchMatch( void );
void TouchGetxy( TOUCH_ATTR *ptouchattr );

#endif
