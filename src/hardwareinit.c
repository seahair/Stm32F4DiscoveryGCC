#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "iwdg.h"
#include "time3.h"
#include "pwm.h"


void LED_Init( void )
{

	LedRed.pin = LEDRED;
	LedRed.LedInit( &LedRed );

	LedGreen.pin = LEDGREEN;
	LedRed.LedInit( &LedGreen );

#if 0
//	LedRed.pin = LEDRED;
	LedRed.LedInit = LedInitRed;
	LedRed.LedON = LedOnRed;
	LedRed.LedOFF = LedOffRed;
	LedRed.SetValue = SetValueLedRed;
	LedRed.GetLedStatus = GetLedStatusRed;
	LedRed.LedBlink = LedBlinkRed;
	LedRed.LedRollBack = LedRollBackRed;
	LedRed.LedInit( );

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

void BEEP_INIT( void )
{
	Beep.BeepInit = BeepInit ;
	Beep.BeepOn = BeepOn ;
	Beep.BeepOff = BeepOff ;
	Beep.BeepOnHzTime = BeepOnHzTime ;
	Beep.PlayMusic = PlayMusic ;

	Beep.BeepInit( );
}


void HardInit( void )
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	delay_init( 168 );
	uart_init( 115200 );

	LED_Init( );
	BEEP_INIT();
	KeyPinInit( );
	KeyExtiInit( );
	//IwdgStart1S( );
	Time3InitMs( 1000 );
	//PwmInit( );
//	TIM14_PWM_Init(500-1,84-1);
//	Time14PwmInit_HZ( 2000 );
//	TIM5_CH1_Cap_Init(0XFFFFFFFF, 84-1);
//	PadInit( 8 );
//	FSMC_SRAM_Init( );
}

