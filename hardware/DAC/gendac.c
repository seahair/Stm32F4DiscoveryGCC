#include "dac.h"
#include "gendac.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

static DAC_ATTR dac_attr; 

static void Gendac_GpioInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//①使能 PA 时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//①初始化 GPIO
	
}

static void Gendac_Dac1Init( void )
{
	DAC_InitTypeDef DAC_InitType;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//②使能 DAC 时钟

 	DAC_Init(DAC_Channel_1,&dac_attr.DACInitType); //③初始化 DAC 通道 1

}


static s8 Gendac_Init( void )
{
	dac_attr.DACInitType.DAC_Trigger=DAC_Trigger_None;//不使用触发功能 TEN1=0	
	dac_attr.DACInitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生
	dac_attr.DACInitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;//屏蔽、幅值设置
	dac_attr.DACInitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable;//输出缓存关闭
	dac_attr.datalenface = DAC_Align_12b_R;
	dac_attr.refv = 3.3;
	
	Gendac_GpioInit( );
	Gendac_Dac1Init( );	
	
	DAC_Cmd(DAC_Channel_1, ENABLE); //④使能 DAC 通道 1
	DAC_SetChannel1Data(dac_attr.datalenface, 0); //12 位右对齐数据格式,初始化为0
}

static void Gendac_Ioctrl( u8 cmd, DAC_ATTR *pdacattr )
{
	switch ( cmd )
	{
		case DACCMDENABLE:
			DAC_Cmd(DAC_Channel_1, ENABLE); //④使能 DAC 通道 1
			break;
		case DACCMDDISABLE:
			DAC_Cmd(DAC_Channel_1, DISABLE); //关闭 DAC 通道 1
			break;
		case DACCMDDATALENFACE:
			dac_attr.datalenface = pdacattr->datalenface;
			break;
		case DACSETREFV:
			dac_attr.refv = pdacattr->refv;
			break;
	}
}

static void Gendac_Write( u16 dacnum )
{
	double temp = dacnum;
	temp/=1000;
	temp=temp*DACRESOLUTION12/dac_attr.refv;
	DAC_SetChannel1Data(dac_attr.datalenface,temp);
}

const DAC_DIR gendac_module = {
	Gendac_Init,
	Gendac_Ioctrl,
	Gendac_Write
};

