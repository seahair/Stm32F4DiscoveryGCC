#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "capture.h"






//=0: CaptureFinsh
//=1: CaptureStart
//=2: Capturewait
//=3: CaptureTimeout
//u8  CaptureStatus = 0; 


void CapturePinInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度 100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //下拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //PA0 复用位定时器 5
}

void CaptureTimeInit1us( u32 per )
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); 

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = CAPTUREHZ-1; //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period = per; //per+1; //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
}

void CaptureInterruptInit( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);//允许更新和捕获中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//响应优先级 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure);
}

void CaptureInit( void )
{
	CapturePinInit( );
	CaptureTimeInit1us( CAPTURENUM );

	TIM_ICInitTypeDef TIM5_ICInitStructure;

	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //选择输入端 IC1 映射到 TI1 上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到 TI1 上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; //配置输入分频,不分频
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	CaptureInterruptInit( );
}

void CaptureStart( void )
{
	TIM_Cmd(TIM5, ENABLE ); //使能定时器 5
	//CaptureStatus = CAPTURESTART;

}

void CaptureStop( void )
{
	TIM_Cmd(TIM5, DISABLE ); //定时器 5
	CaptureStatus = CAPTUREFINSH;
}

u32 CaptureGetValue( void )
{
    u32 temp = TIM_GetCapture1(TIM5);
	return temp;
}

uint64_t CaptureClacTime( u32 num )
{
	uint64_t tmp = (CaptureCount*654 + num);
	return tmp;
}










