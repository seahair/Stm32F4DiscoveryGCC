#include "adc.h"
#include "extgadc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"

#define  ARRAYLEN(array)	(sizeof(array)/sizeof(array[0]))

const ADC_DIR *adc_dir = 0;

ADC_DIR *adc_modules[] = {
	&extgadc_module,
};



s8 AdcInit( u8 adcdev )
{
	if( adcdev < ARRAYLEN(adc_modules) )
	{
		adc_dir = adc_modules[adcdev];
		adc_dir->init( );
			
		return 0;
	}

	return -1;
}


void AdcIoctrl( u8 cmd, ADC_ATTR *padcattr )
{
	if( adc_dir )
		return adc_dir->ioctrl( cmd, padcattr );
}

float AdcRead( void )
{
	if( adc_dir )
		return adc_dir->read( );
}








