#ifndef  __ADC_H__
#define  __ADC_H__

#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"


#define  ADCRESOLUTION12    4096 


typedef enum {
	EXTGADC = 0,
	INTEMPERATE,
	LIGHTSENSE
}ADC_DEV;


typedef enum {
	ADCCMDENABLE = 0,
	ADCCMDDISABLE,
	ADCCMDSETREFV,
	ADCCMDSERCH,
	ADCCMDSETCOUNT
}ADC_CMD;

typedef struct _ADC_ATTR{
	ADC_CommonInitTypeDef cominit;
	ADC_InitTypeDef	 init;
	u8 channel;
	float refv;
	u16 count;
}ADC_ATTR;

typedef struct _ADC_DIR{
	s8   (*init) ( void );
	void (*ioctrl) ( u8 cmd, ADC_ATTR *padcattr );
	float (*read) ( void );
}ADC_DIR;

s8 AdcInit( u8 adcdev );
void AdcIoctrl( u8 cmd, ADC_ATTR *padcattr );
float AdcRead( void );


#endif 
