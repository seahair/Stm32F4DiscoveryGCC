#include "dma.h"
#include "stm32f4xx.h"
#include "usart1txdma.h"


static const DMA_DIR *dma_dir;

const DMA_DIR *dma_modules[] = {
	&usart1tx_module,
};


s8 DmaInit( u8 dmadev, DMA_ATTR *pdmaattr )
{
	dma_dir = dma_modules[dmadev];
	dma_dir->init( pdmaattr );
}

void DmaIoCtrl(  u8 cmd, DMA_ATTR *pdmaattr )
{
	if( dma_dir )
		return dma_dir->ioctrl( cmd, pdmaattr );
}

u8   DmaGetStatus ( void )
{
	if( dma_dir )
		return dma_dir->getstatus( );
}

float DmaGetTransProgess( void )
{
	if( dma_dir )
		return dma_dir->gettransprogess( );
}




