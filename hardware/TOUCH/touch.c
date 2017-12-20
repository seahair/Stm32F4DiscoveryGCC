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

		//if( touch_dir )
		{
				if( 0 == TouchMatch() )
				{
						touch_dir->init( );	
						return 0;
				}
				else
						return -1;
		}
		//else
		//		return -2;
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

static void TouchXYChange( TOUCH_ATTR *ptouchattr )
{
	u8 i;

	for( i=0; i<ptouchattr->touchnum; i++ )
	{
		ptouchattr->LCDXY.x[i] = ptouchattr->OriginalXY.x[i];	
		ptouchattr->LCDXY.y[i] = ptouchattr->OriginalXY.y[i];	
	}
}

void TouchGetxy( TOUCH_ATTR *ptouchattr )
{
	//if( touch_dir )
	{
		touch_dir->getxy( ptouchattr );

		TouchXYChange( ptouchattr );

		for( u8 i=0; i<ptouchattr->touchnum; i++ )
		{
				printf("Touch [%d] X is: 0X%d \r\n", i, ptouchattr->OriginalXY.x[i]);
				printf("Touch [%d] Y is: 0X%d \r\n", i, ptouchattr->OriginalXY.y[i]);
		}
	}
}

