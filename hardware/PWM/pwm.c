#include "pwm.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


static u16 Period=65535;


static void PwmPinInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOF,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_TIM14); 

}

static void PwmTimeInit1Us( u16 per )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = PWMHZ-1 ; //定时器分频 84MHz
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = per-1; //自动重装载值 500
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseStructure);
}

void PwmInit( void )
{
	TIM_OCInitTypeDef TIM_OCInitStructure;

	PwmPinInit( );
	PwmTimeInit1Us( PWMPERIOD );

	//初始化 TIM14 Channel1 PWM 模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM 调制模式 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性低
	TIM_OC1Init(TIM14, &TIM_OCInitStructure); //初始化外设 TIM1 4OC1

	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable); //使能预装载寄存器
	TIM_ARRPreloadConfig(TIM14,ENABLE);//ARPE 使能
}

void PwmSetPeriodUs( u16 time )
{
	if( time < 65535 )
	{
		PwmTimeInit1Us( time );	
		Period = time;
	}
}

void PwmSetDutyCycle( u16 num )
{
	if( num < Period )
	{
		TIM_SetCompare1( TIM14, num );
	}
}

void PwmStart( void )
{
	TIM_Cmd(TIM14, ENABLE); 
}

void PwmStop( void )
{
	TIM_Cmd(TIM14, DISABLE); 
}






