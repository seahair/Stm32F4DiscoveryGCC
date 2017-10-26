#include "stm32f4xx.h"
#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
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


int main(int argc, char *argv[])
{

	HardInit();
	while(1)
	{
		delay_ms(10);

	//    LedBlink( LEDGREEN );
//		LedRed.LedBlink = LedBlinkRed;
		LedRed.LedBlink( &LedRed, 1000 );
		LedGreen.LedBlink( &LedGreen, 500 );

		Beep.BeepOnHzTime( 200, 2000 );
		Beep.PlayMusic( );

	}
}

	


