#ifndef  __DAC_H__
#define  __DAC_H__

#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"

#define  DACRESOLUTION12    4096 
#define  DACRESOLUTION8	    256 

typedef enum {
	GENDAC = 0,
	PWMDAC
}DAC_DEV;


typedef enum {
	DACCMDENABLE = 0,
	DACCMDDISABLE,
	DACCMDDATALENFACE,
	DACSETREFV,
	DACSETPSC,
	DACSETARR,
}DAC_CMD;

typedef struct _DAC_ATTR{
	DAC_InitTypeDef DACInitType;
	u32 datalenface;
	float refv;
	u16 psc;
	u16 arr;
}DAC_ATTR;

typedef struct _DAC_DIR{
	s8 (*init) ( void );
	void (*ioctrl) ( u8 cmd, DAC_ATTR *pdacattr );
	void (*write) ( u16 );
}DAC_DIR;

s8 DacInit( u8 dacdev );
void DacIoctrl( u8 cmd, DAC_ATTR *pdacattr );
void DacWrite( u16 dacnum );





#endif
