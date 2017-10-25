#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
//#include "stm32f4xx_rcc.h"


void LED_Init_G( void )
{
//	LedRed.pin = LEDRED;
	LedRed.LedInit = LedInitRed;
	LedRed.LedON = LedOnRed;
	LedRed.LedOFF = LedOffRed;
	LedRed.SetValue = SetValueLedRed;
	LedRed.GetLedStatus = GetLedStatusRed;
	LedRed.LedBlink = LedBlinkRed;
	LedRed.LedRollBack = LedRollBackRed;
	LedRed.LedInit( );

#if 1
//	LedGreen.pin = LEDGREEN;
	LedGreen.LedInit = LedInitGreen;
	LedGreen.LedON = LedOnGreen;
	LedGreen.LedOFF = LedOffGreen;
	LedGreen.GetLedStatus = GetLedStatusGreen;
	LedGreen.LedBlink = LedBlinkGreen;
	LedGreen.LedRollBack = LedBlinkGreen;
	LedGreen.LedInit( );
#endif 	
	
}

void HardInit( void )
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init( 168 );
	uart_init( 115200 );
//	LedInit( LedRed );
//	LedInit( LedGreen );	
//	LED_Init( );
	LED_Init_G( );
//	BEEP_Init();
//	KEY_Init();
//	EXTIX_Init();
//	TIM3_Int_Init(5000-1,8400-1);
//	Tim3Init_ms( 1000 );
//	TIM14_PWM_Init(500-1,84-1);
//	Time14PwmInit_HZ( 2000 );
//	TIM5_CH1_Cap_Init(0XFFFFFFFF, 84-1);
//	PadInit( 8 );
//	FSMC_SRAM_Init( );
}

