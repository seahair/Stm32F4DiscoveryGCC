#include "stm32f4xx.h"
#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "exti.h"
#include "time.h"
#include "pwm.h"
#include "capture.h"
#include <stdio.h>
#include <stdlib.h>
#include "pad.h"
#include "sram.h"*/

#define ADDR 250000

/*
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
*/



//u32 SramTest[ ADDR ]  __attribute__((at(0x68000000)));
//void HardInit( void );

//u8 TIM5CH1_CAPTURE_STA=1;  
//u32 TIM5CH1_CAPTURE_VAL=2;

int main(int argc, char *argv[])
{
	HardInit();
	//u8 i=0;
	long long temp = 0;
	while(1)
	{
		delay_ms(10);

		//PwmTest();
		//PwmTestDuty();
		//KeyTest();
	//    LedBlink( LEDGREEN );
//		LedRed.LedBlink = LedBlinkRed;
		LedRed.LedBlink( &LedRed, 1000 );
		LedGreen.LedBlink( &LedGreen, 500 );
	/*	if( PadScan(0 ) )
		{
			LedON( LedGreen );
			delay_ms( 500 );
		}
		else
			LedON( LedGreen );*/

		//	printf ("hello world\r\n");
		//BeepAlarm( 500 );

		//printf("HIGH:%lld us\r\n", Time5CalcCaptureTime() );//打印总的高点平时间
		//Time5CalcCaptureTime( );
			
		
 		/*if(TIM5CH1_CAPTURE_STA&0X80)        //³É¹Š²¶»ñµœÁËÒ»ŽÎžßµçÆœ
		{
			temp=TIM5CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFFFFFF;		 		         //Òç³öÊ±Œä×ÜºÍ
			temp+=TIM5CH1_CAPTURE_VAL;		   //µÃµœ×ÜµÄžßµçÆœÊ±Œä
			printf("HIGH:%lld us\r\n",temp); //ŽòÓ¡×ÜµÄžßµãÆœÊ±Œä
			TIM5CH1_CAPTURE_STA=0;			     //¿ªÆôÏÂÒ»ŽÎ²¶»ñ
		}*/
		

	}
}

	


