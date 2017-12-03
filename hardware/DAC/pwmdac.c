#include "dac.h"
#include "pwmdac.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

static DAC_ATTR dac_attr; 

static void Pwmdac_GpioInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能 PA 时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化 PA3

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM9);//PA3 复用位定时器 9 AF3
}

static void Pwmdac_TimeInit( void )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,ENABLE); //TIM9 时钟使能 

	TIM_TimeBaseStructure.TIM_Prescaler=dac_attr.psc; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=dac_attr.arr; //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM9,&TIM_TimeBaseStructure);//初始化定时器 9

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性高
	TIM_OCInitStructure.TIM_Pulse=0;
	TIM_OC2Init(TIM9, &TIM_OCInitStructure); //初始化外设 TIM9 OC2

	TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_ARRPreloadConfig(TIM9,ENABLE);//ARPE 使能

}


static s8 Pwmdac_Init( void )
{
	dac_attr.psc = 255;
	dac_attr.arr = 254;

	Pwmdac_GpioInit( );
	Pwmdac_TimeInit( );	
	
	TIM_Cmd(TIM9, ENABLE); //使能 TIM9
	TIM_SetCompare2(TIM9,200); //初始值０
}

static void Pwmdac_Ioctrl( u8 cmd, DAC_ATTR *pdacattr )
{
	switch ( cmd )
	{
		case DACCMDENABLE:
			TIM_Cmd(TIM9, ENABLE); //使能 TIM9
			break;
		case DACCMDDISABLE:
			TIM_Cmd(TIM9, DISABLE); //关闭 TIM9
			break;
		case DACSETPSC:
			dac_attr.psc = pdacattr->psc;
			break;
		case DACSETARR:
			dac_attr.arr = pdacattr->arr;
			break;
	}
}

static void Pwmdac_Write( u16 dacnum )
{
	u8 temp = dacnum&0XFF;

	TIM_SetCompare2(TIM9,temp); 
}

const DAC_DIR pwmdac_module = {
	Pwmdac_Init,
	Pwmdac_Ioctrl,
	Pwmdac_Write
};

