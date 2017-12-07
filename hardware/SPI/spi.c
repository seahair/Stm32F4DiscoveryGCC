#include "spi.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_spi.h"


static SPI_ATTR spi_attr;


static void spi_gpioinit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能 GPIOA 时钟

	GPIO_InitStructure.GPIO_Pin = spi_attr.PinMISO | spi_attr.PinMOSI | spi_attr.PinSCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);// 初始化

	GPIO_PinAFConfig(GPIOB,spi_attr.PinSCK,GPIO_AF_SPI1); //PB3 复用为 SPI1
	GPIO_PinAFConfig(GPIOB,spi_attr.PinMISO,GPIO_AF_SPI1); //PB4 复用为 SPI1
	GPIO_PinAFConfig(GPIOB,spi_attr.PinMOSI,GPIO_AF_SPI1); //PB5 复用为 SPI1
}

static void spi_structinit( void )
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);// 使能 SPI1 时钟

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,ENABLE);//复位 SPI1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1,DISABLE);//停止复位 SPI1

	SPI_Init(SPI1, &spi_attr.spitype);

	SPI_Cmd(SPI1, ENABLE); //使能 SPI1
}

s8 SpiInit( SPI_ATTR *pspiattr )
{
	spi_attr.spitype.SPI_Direction = pspiattr->spitype.SPI_Direction; 
	spi_attr.spitype.SPI_Mode = pspiattr->spitype.SPI_Mode; 
	spi_attr.spitype.SPI_DataSize = pspiattr->spitype.SPI_DataSize; 
	spi_attr.spitype.SPI_CPOL = pspiattr->spitype.SPI_CPOL; 
	spi_attr.spitype.SPI_CPHA = pspiattr->spitype.SPI_CPHA; 
	spi_attr.spitype.SPI_NSS = pspiattr->spitype.SPI_NSS; 
	spi_attr.spitype.SPI_BaudRatePrescaler = pspiattr->spitype.SPI_BaudRatePrescaler; 
	spi_attr.spitype.SPI_FirstBit = pspiattr->spitype.SPI_FirstBit; 
	spi_attr.spitype.SPI_CRCPolynomial = pspiattr->spitype.SPI_CRCPolynomial; 

	spi_gpioinit( );
	spi_structinit( );
	
	return 0;
}

void SpiIoctrl( u8 cmd, SPI_ATTR *pspiattr )
{
	switch ( cmd )
	{
		case SPICMDENABLE:
			SPI_Cmd(SPI1, ENABLE); //使能 SPI1
			break;
		case SPICMDDISABLE:
			SPI_Cmd(SPI1, DISABLE); //关闭 SPI1
			break;
		case SPISETSPEED:
			spi_attr.spitype.SPI_BaudRatePrescaler = pspiattr->spitype.SPI_BaudRatePrescaler; 
			SPI_Init(SPI1, &spi_attr.spitype);
			SPI_Cmd(SPI1, ENABLE); //使能 SPI1
			break;
		default:
			break;
	}
}

u8 SPI1_ReadWriteByte(u8 TxData)
{					 

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//µÈŽý·¢ËÍÇø¿Õ  
	SPI_I2S_SendData(SPI1, TxData); //Íš¹ýÍâÉèSPIx·¢ËÍÒ»žöbyte  ÊýŸÝ

	while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET){} //µÈŽýœÓÊÕÍêÒ»žöbyte  
	return SPI_I2S_ReceiveData(SPI1); //·µ»ØÍš¹ýSPIx×îœüœÓÊÕµÄÊýŸÝ	
}
