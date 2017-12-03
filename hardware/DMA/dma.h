#ifndef  __DMA_H__
#define  __DMA_H__

#include "stm32f4xx.h"


typedef enum {
	DMAUSART1TX = 0,
}DMA_DEV;

typedef enum {
	DMACMDSTART,
	DMACMDSTOP,
	DMACMDCLEARFLAG,
	DMACMDSETSTREAM,
	DMACMDSETFLAG,
	DMACMDSETCHN,
	DMACMDSETPERADDR,
	DMACMDSETMEMADDR,
	DMACMDSETTXSIZE,
}DMA_CMD;

typedef struct _DMA_ATTR {
	DMA_Stream_TypeDef *DMA_Streamx; 
	u32 flag;
	u32 channel;
	u32 periphaddr;
	u32 memoryaddr;
	u32 txsize;
}DMA_ATTR;

typedef struct _DMA_DIR {
	s8   (*init) ( DMA_ATTR *pdmaattr );
	void (*ioctrl) ( u8 cmd, DMA_ATTR *pdmaattr );
	u8 (*getstatus) ( void );
	float (*gettransprogess) ( void );
}DMA_DIR;

s8 DmaInit( u8 dmadev, DMA_ATTR *pdmaattr );
void DmaIoCtrl( u8 cmd, DMA_ATTR *pdmaattr );
u8   DmaGetStatus ( void );
float DmaGetTransProgess( void );






#endif 
