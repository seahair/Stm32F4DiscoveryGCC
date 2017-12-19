#include "touch.h"
#include "gt9147.h"
#include "stm32f4xx.h"




const TOUCH_DIR *touch_dir = 0;

TOUCH_DIR *touch_modules[] = {
	&gt9147_module,
};

#define  TOUCHDMODULELEN	(sizeof(touch_modules)/sizeof(touch_modules[0]))


s8 TouchInit( )
{
	//I2c_init( );

	if( 0 == TouchMatch() )
	{
		touch_dir->init( );	
		return 0;
	}
	else
		return -1;
}

s8 TouchMatch( void )
{
	u8 i;

	for( i=0; i<TOUCHDMODULELEN; i++ )
	{
		touch_dir = touch_modules[i];
		if( 0 == touch_dir->match() )
			return 0;
	}

	return -1;
}

void TouchGetxy( TOUCH_ATTR *ptouchattr )
{
	touch_dir->getxy( ptouchattr );

	for( u8 i=0; i<ptouchattr->touchnum; i++ )
	{
		printf("Touch [%d] X is: 0X%x \r\n", i, ptouchattr->OriginalXY.x[i]);
		printf("Touch [%d] Y is: 0X%x \r\n", i, ptouchattr->OriginalXY.y[i]);
	}
}

