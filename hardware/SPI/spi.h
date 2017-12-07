#ifndef  __SPI_H__
#define  __SPI_H__

#include "stm32f4xx.h"
#include "stm32f4xx_spi.h"


typedef enum {
	SPICMDENABLE = 0,
	SPICMDDISABLE,
	SPISETSPEED,
}SPI_CMD;


typedef struct _SPI_ATTR{
	SPI_InitTypeDef spitype;
	u16  PinMISO;
	u16  PinMOSI;
	u16  PinSCK;
	u16  PinCS;
}SPI_ATTR;

s8 SpiInit( SPI_ATTR *pspiattr );
void SpiIoctrl( u8 cmd, SPI_ATTR *pspiattr );
u8 SPI1_ReadWriteByte(u8 TxData);



#endif 
