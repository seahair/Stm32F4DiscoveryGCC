#include "dma.h"
#include "usart1txdma.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"


static DMA_ATTR dma_attr;


static void usart1tx_gpioinit( void )
{
	//此部分已在串口调试终端中初始化
}

static void usart1tx_usart1init( void )
{
	//此部分已在串口调试终端中初始化
}

static void usart1tx_dmainit( void )
{
	DMA_InitTypeDef  DMA_InitStructure;

	if((u32)dma_attr.DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能 
	}else 
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
	DMA_DeInit(dma_attr.DMA_Streamx);

	DMA_InitStructure.DMA_Channel = dma_attr.channel;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = dma_attr.periphaddr;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = dma_attr.memoryaddr;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = dma_attr.txsize;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(dma_attr.DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream

	while (DMA_GetCmdStatus(dma_attr.DMA_Streamx) != DISABLE){}//等待DMA可配置 
}

static s8 usart1tx_init( DMA_ATTR *pdmaattr )
{
	dma_attr.DMA_Streamx = DMA2_Stream7;
	dma_attr.flag = DMA_FLAG_TCIF7;
	dma_attr.channel = DMA_Channel_4;
	dma_attr.periphaddr = (u32)&USART1->DR;
	dma_attr.memoryaddr = pdmaattr->memoryaddr;
	dma_attr.txsize = 820;

	usart1tx_gpioinit( );
	usart1tx_usart1init( );
	usart1tx_dmainit( );

}

static void usart1tx_ioctrl( u8 cmd, DMA_ATTR *pdmaattr )
{
	switch ( cmd )
	{
		case DMACMDSTART :
			USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
			DMA_Cmd(dma_attr.DMA_Streamx, DISABLE);                      //关闭DMA传输 
			while (DMA_GetCmdStatus(dma_attr.DMA_Streamx) != DISABLE){}	//确保DMA可以被设置  
			DMA_SetCurrDataCounter(dma_attr.DMA_Streamx,dma_attr.txsize);          //数据传输量  
			DMA_Cmd(dma_attr.DMA_Streamx, ENABLE);                      //开启DMA传输 
			break;
		case DMACMDSTOP :
			USART_DMACmd(USART1,USART_DMAReq_Tx,DISABLE);
			DMA_Cmd(dma_attr.DMA_Streamx, DISABLE);                      //关闭DMA传输 
			break;
		case DMACMDSETSTREAM :
			dma_attr.DMA_Streamx = pdmaattr->DMA_Streamx;
			break;
		case DMACMDSETFLAG :
			dma_attr.flag = pdmaattr->flag;
			break;
		case DMACMDSETCHN :
			dma_attr.channel = pdmaattr->channel;
			break;
		case DMACMDSETPERADDR :
			dma_attr.periphaddr = pdmaattr->periphaddr;
			break;
		case DMACMDSETMEMADDR :
			dma_attr.memoryaddr = pdmaattr->memoryaddr;
			break;
		case DMACMDSETTXSIZE :
			dma_attr.txsize = pdmaattr->txsize;
			break;
		case DMACMDCLEARFLAG :
			DMA_ClearFlag( dma_attr.DMA_Streamx, dma_attr.flag );//清除DMA2_Steam7传输完成标志
			break;
	}
}

static u8 usart1tx_getstatus( void )
{
	return DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7);	
}

static float usart1tx_gettransprogess( void )
{
	float temp = DMA_GetCurrDataCounter(DMA2_Stream7);

	return temp/dma_attr.txsize;
}

const DMA_DIR usart1tx_module = {
	usart1tx_init,
	usart1tx_ioctrl,
	usart1tx_getstatus,
	usart1tx_gettransprogess
};
