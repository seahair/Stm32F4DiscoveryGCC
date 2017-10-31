#include "stm32f4xx.h"
#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "time3.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
#include "delay.h"
#include "exti.h"
#include "pwm.h"
#include "capture.h"
#include <stdio.h>
#include <stdlib.h>
#include "pad.h"
#include "sram.h"*/

void MyKeyTest( u8 key );

int main(int argc, char *argv[])
{

	HardInit();
	while(1)
	{
		delay_ms(1000);
		//KeyTest( MyKeyTest );	
		LedGreen.LedRollBack( &LedGreen );
	}
}

	
void MyKeyTest( u8 key )
{
	switch (key)
	{
		case KEY0:
				LedRed.LedBlink( &LedRed, 1000 );
			break;
		case KEY1:
				LedGreen.LedBlink( &LedGreen, 5000 );
			break;
		case KEY2:
				LedRed.LedOFF( &LedRed );
			break;
		case KEYWK:
				Beep.PlayMusic( );	
			break;
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		LedRed.LedRollBack( &LedRed );
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //清除中断标志位
}
