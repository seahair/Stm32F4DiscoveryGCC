#include "key.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"


void KeyPinInit( void )
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PINKEY0|PINKEY1|PINKEY2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PINKEYWK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}


s8 GetKeyStatus( u8 pin )
{
	switch (pin)
	{
		case KEY0:
			return GPIO_ReadInputDataBit( GPIOE, PINKEY0 );
			break;
		case KEY1:
			return GPIO_ReadInputDataBit( GPIOE, PINKEY1 );
			break;
		case KEY2:
			return GPIO_ReadInputDataBit( GPIOE, PINKEY2 );
			break;
		case KEYWK:
			return GPIO_ReadInputDataBit( GPIOA, PINKEYWK );
			break;
		default:
			return -1;
	}
}

u8 WhichKeyPress( void )
{
	while(1)
	{
		if( KEY0PRESS == GetKeyStatus( KEY0 ) )
		{
			delay_ms(100);
			if( KEY0PRESS == GetKeyStatus( KEY0 ) )
				return KEY0;
		}

		if( KEY1PRESS == GetKeyStatus( KEY1 ) )
		{
			delay_ms(100);
			if( KEY1PRESS == GetKeyStatus( KEY1 ) )
				return KEY1;
		}

		if( KEY2PRESS == GetKeyStatus( KEY2 ) )
		{
			delay_ms(100);
			if( KEY0PRESS == GetKeyStatus( KEY2 ) )
				return KEY2;
		}

		if( KEYWKPRESS == GetKeyStatus( KEYWK ) )
		{
			delay_ms(100);
			if( KEYWKPRESS == GetKeyStatus( KEYWK ) )
				return KEYWK;
		}

	}
}


void KeyTest( KEYTEST pFun )
{
	KEYTEST  KeyTestHandle = pFun;
	KeyTestHandle( WhichKeyPress() );
}

void KeyExtiInit( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	KeyPinInit( );

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);//PE2 连接线 2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);//PE3 连接线 3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);//PE4 连接线 4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

	/* 配置 EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能 LINE0
	EXTI_Init(&EXTI_InitStructure);

	/* 配置 EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//中断线使能
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级 0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置 NVIC

	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//外部中断 2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;//抢占优先级 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;//外部中断 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置 NVIC

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断 4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;//抢占优先级 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//响应优先级 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置 NVIC 

}





