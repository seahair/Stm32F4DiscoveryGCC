#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "iwdg.h"
//#include "stm32f4xx_rcc.h"


void LED_Init( void )
{
	printf("LedRed Addr is 0x%x\r\n", &LedRed);
	LedRed.pin = LEDRED;
	LedRed.LedInit = LedInit;
	LedRed.LedON = LedOn;
	LedRed.LedOFF = LedOff;
	LedRed.SetValue = SetValue;
	LedRed.GetLedStatus = GetLedStatus;
	LedRed.LedBlink = LedBlink;
	LedRed.LedRollBack = LedRollBack;
	LedRed.LedInit( &LedRed );

	LedGreen.pin = LEDGREEN;
	LedGreen.LedInit = LedInit;
	LedGreen.LedON = LedOn;
	LedGreen.LedOFF = LedOff;
	LedGreen.SetValue = SetValue;
	LedGreen.GetLedStatus = GetLedStatus;
	LedGreen.LedBlink = LedBlink;
	LedGreen.LedRollBack = LedBlink;
	LedGreen.LedInit( &LedGreen );

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
<<<<<<< HEAD

	LED_Init( );
	BEEP_INIT();
	KeyPinInit( );
	IwdgStart1S( );
=======
//	LedInit( LedRed );
//	LedInit( LedGreen );	
	LED_Init( );
	BEEP_INIT();
	KeyPinInit( );
//	EXTIX_Init();
>>>>>>> f95aa9d0679305427ea9271eb560c1da698539a0
//	TIM3_Int_Init(5000-1,8400-1);
//	Tim3Init_ms( 1000 );
//	TIM14_PWM_Init(500-1,84-1);
//	Time14PwmInit_HZ( 2000 );
//	TIM5_CH1_Cap_Init(0XFFFFFFFF, 84-1);
//	PadInit( 8 );
//	FSMC_SRAM_Init( );
}

