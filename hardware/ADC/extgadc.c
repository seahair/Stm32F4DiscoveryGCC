#include "adc.h"
#include "extgadc.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"


static ADC_ATTR adc_attr;

static void extgadc_gpioinit( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//PA5 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
}

static void extgadc_adcinit( void )
{
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 

	ADC_CommonInit( &adc_attr.cominit );//common初始化
	ADC_Init( ADC1, &adc_attr.init );//ADC初始化
}



static s8 extgadc_init( void )
{
	adc_attr.cominit.ADC_Mode = ADC_Mode_Independent;//独立模式
	adc_attr.cominit.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
	adc_attr.cominit.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
	adc_attr.cominit.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz	
	adc_attr.init.ADC_Resolution = ADC_Resolution_12b;//12位模式
	adc_attr.init.ADC_ScanConvMode = DISABLE;//非扫描模式
	adc_attr.init.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
	adc_attr.init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	adc_attr.init.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	adc_attr.init.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 

	adc_attr.channel = ADC_Channel_5;
	adc_attr.refv = 3.3;
	adc_attr.count = 10;

	extgadc_gpioinit( );
	extgadc_adcinit( );
}

static void extgadc_ioctrl( u8 cmd, ADC_ATTR *padcattr )
{
	switch (cmd)
	{
		case ADCCMDENABLE:
			break;
		case ADCCMDDISABLE:
			break;
		case ADCCMDSETREFV:
			adc_attr.refv = padcattr->refv;
			break;
		case ADCCMDSERCH:
			adc_attr.channel = padcattr->channel; 
			break;
		case ADCCMDSETCOUNT:
			adc_attr.count = padcattr->count;
			break;
	}
}

static u16 extgadc_getvalue( void )
{
	ADC_RegularChannelConfig(ADC1, adc_attr.channel, 1, ADC_SampleTime_480Cycles );//ADC1,通道,480个周期,提高采样时间可以提高精确度	 
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}

static u16 extgadc_getavgvalue( void )
{
	u32 temp_value = 0;

	for( u8 t=0; t<adc_attr.count; t++ )
	{
		temp_value = extgadc_getvalue( );
		delay_ms( 5 );
	}

	return temp_value/adc_attr.count;
}


static float extgadc_read( void )
{
	float value = 0;

	value = ((extgadc_getavgvalue()*adc_attr.refv)/ADCRESOLUTION12);

	return value;
}



const ADC_DIR extgadc_module = {
	extgadc_init,
	extgadc_ioctrl,
	extgadc_read
};
