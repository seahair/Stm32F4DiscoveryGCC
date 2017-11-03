#include "tpad.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"




static void TpadPinInitIO( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TPADPIN;  //PA5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍšÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //ËÙ¶È100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; //ÏÂÀ­
    GPIO_Init(GPIOA,&GPIO_InitStructure); //³õÊŒ»¯PA5
    
}

static void TpadPinPullDown( void )
{
    GPIO_ResetBits(GPIOA, TPADPIN);   
}

static void TpadPinInitCap( void )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TPADPIN; //PA5
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//žŽÓÃÊä³ö
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//ËÙ¶È100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //ÍÆÍì 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//²»ŽøÉÏÏÂÀ­ 
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);
}

static void TpadTimeInit1us( u32 per )
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  

    TIM_TimeBaseStructure.TIM_Period = per; 
    TIM_TimeBaseStructure.TIM_Prescaler = TPADHZ-1 ;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
}

static void TpadInterruptInit( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);//允许更新和捕获中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;//响应优先级 0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道使能
	NVIC_Init(&NVIC_InitStructure);
}

void TpadInit( void )
{
    TIM_ICInitTypeDef  TIM2_ICInitStructure;

    TpadPinInitCap( );
    TpadTimeInit1us( );

    TIM2_ICInitStructure.TIM_Channel = TIM_Channel_1; 
    TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  
    TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
    TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM2_ICInitStructure.TIM_ICFilter = 0x00;
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);

    TpadInterruptInit( );
}

void TpadClear( void )
{
    TpadPinInitIO( );
    TpadPinPullDown( void );
    delay_ms( 5 );    
    TpadPinInitCap( );
}

void TpadInterruptStart( void )
{
    TIM_Cmd(TIM2, ENABLE );
}

void TpadInterruptStop( void )
{
    TIM_Cmd(TIM2, DISABLE );
}

u32 TpadGetCapTime( void )
{

}












