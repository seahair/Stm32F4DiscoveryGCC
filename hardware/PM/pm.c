#include "pm.h"
#include "wakeup.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"


#define  ARRAYLEN(array)	(sizeof(array)/sizeof(array[0]))

const PM_DIR *pm_dir=0;


const static PM_DIR *pm_modules[] = {
	&wakeup_module,

};


static s8  PmMatch( void )
{
	for( u8 i=0; i<ARRAYLEN(pm_modules); i++ )
	{
		pm_dir = pm_modules[i];
		return 0;
	}

}


s8 PmInit( void )
{
	if( 0 == PmMatch( ) )
	{
		pm_dir->init( );
		return 0;
	}

	return -1;
}

void PmIoctrl( u8 cmd, PM_ATTR *ppmattr )
{
	if( pm_dir )
		pm_dir->ioctrl( cmd, ppmattr );
}

s8 PmRead( void )
{
	if( pm_dir )
		pm_dir->read( );
}




