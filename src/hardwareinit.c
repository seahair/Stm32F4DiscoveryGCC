#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "iwdg.h"
#include "time3.h"
#include "pwm.h"
#include "capture.h"
#include "tpad.h"
#include "lcd.h"
#include "rtc.h"
#include "randomnum.h"
#include "pm.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "at24cxx.h"


void LED_Init( void )
{

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
    LedGreen.LedRollBack = LedRollBack;
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

	LED_Init( );
	BEEP_INIT();
	KeyPinInit( );
    LcdInit( );
	RtcInit( );
	//KeyExtiInit( );
	//IwdgStart1S( );
	//Time3InitMs( 500 );
	//PwmInit( );
//	CaptureInit( );
	//TpadInit( );
	//RnInit( );
	//PmInit( );
	//AdcInit( EXTGADC );
	//AdcInit( INTEMPERATE );
	//AdcInit( LIGHTSENSE );
	//DacInit( GENDAC );	
	//DacInit( PWMDAC );	
	//DmaInit( DMAUSART1TX );
	at24cxx_init( );

//	FSMC_SRAM_Init( );
}

