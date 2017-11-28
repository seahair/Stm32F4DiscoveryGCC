#include "pm.h"
#include "wakeup.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_pwr.h"
#include "delay.h"


static PM_ATTR pm_attr;

static void wakeup_gpioinit( void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化	
}

static void wakeup_extiinit( void )
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);//PA0 连接到中断线0


	EXTI_InitStructure.EXTI_Line = EXTI_Line0;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置


	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//外部中断0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置NVIC
}

static s8 wakeup_init( void )
{
	wakeup_gpioinit( );
	wakeup_extiinit( );

	return 0;
}

static void wakeup_ioctrl( u8 cmd, PM_ATTR *ppmattr )
{
	switch ( cmd )
	{
		case PMCMDENABLE :
			PWR_WakeUpPinCmd(ENABLE);			
			break;
		case PMCMDDISABLE :
			PWR_WakeUpPinCmd(DISABLE);
			break;
		case PMCMDSTART :
			RCC_AHB1PeriphResetCmd(0X04FF,ENABLE);//复位所有IO口
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟		 
			PWR_BackupAccessCmd(ENABLE);//后备区域访问使能

			RTC_ITConfig(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA,DISABLE);//关闭RTC相关中断，可能在RTC实验打开了。
			RTC_ClearITPendingBit(RTC_IT_TS|RTC_IT_WUT|RTC_IT_ALRB|RTC_IT_ALRA);//清楚RTC相关中断标志位。
			PWR_ClearFlag(PWR_FLAG_WU);//清除Wake-up 标志
			PWR_WakeUpPinCmd(ENABLE);//设置WKUP用于唤醒
			PWR_EnterSTANDBYMode();	//进入待机模式
			break;
		case PMCMDSTOP :
			break;
		case PMCMDSETHOLDTIME :
			pm_attr.holdtime = ppmattr->holdtime;
			break;
	}
}

static u8 wakeup_getextistatus( void )
{
	return GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0);
}

static s8 wakeup_read( void )
{
	u8 t=0;
	u8 tx=0;
	while( 1 )
	{
		if( KEYWAKEUPPRESSDOWN == wakeup_getextistatus() )
		{
			t++;
			if( t>(pm_attr.holdtime/30) )
			{
				return 1;
			}
		}
		else
		{
			tx++;
			if(tx>3)
				return 0;
		}
		
		delay_ms( 30 );
	}
}

const PM_DIR wakeup_module = {
	wakeup_init,
	wakeup_ioctrl,
	wakeup_read
};


