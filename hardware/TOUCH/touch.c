#include "touch.h"
#include "stm32f4xx.h"




const TOUCH_DIR *touch_dir = 0;

TOUCH_DIR *touch_modules = {
	&gt9147_module,
};

#define  TOUCHDMODULELEN	(sizeof(touch_modules)/sizeof(touch_modules[0]))


s8 TouchInit( TOUCH_ATTR *ptouchattr )
{
	//I2c_init( );

	if( 0 == TouchMatch() )
	{
		touch_dir->init( ptouchattr );	
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

void TouchGetxy( u16 *x, u16 *y )
{
	touch_dir->getxy( x, y );
}

