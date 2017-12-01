#include "dac.h"
#include "gendac.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"


const DAC_DIR *dac_dir = 0;

static DAC_DIR *dac_modules[] = {
	&gendac_module,
	//&pwmdac_module
};


s8 DacInit( u8 dacdev )
{
	dac_dir = dac_modules[dacdev];
	if( dac_dir )
		dac_dir->init( );
}

void DacIoctrl( u8 cmd, DAC_ATTR *pdacattr )
{
	if( dac_dir )
		dac_dir->ioctrl( cmd, pdacattr );
}

void DacWrite( u16 dacnum )
{
	if( dac_dir )
		dac_dir->write( dacnum );
}












