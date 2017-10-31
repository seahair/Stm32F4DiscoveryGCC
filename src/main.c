#include "stm32f4xx.h"
#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "time3.h"
#include "pwm.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
#include "delay.h"
#include "exti.h"
#include "capture.h"
#include <stdio.h>
#include <stdlib.h>
#include "pad.h"
#include "sram.h"*/

u16 DutyCycle = 100;
u16 Period = 500;

void MyKeyTest( u8 key );

int main(int argc, char *argv[])
{
	HardInit();
    //Time3Start( );
    PwmStart( );
	while(1)
	{
		//delay_ms(1000);
		KeyTest( MyKeyTest );	
		//LedGreen.LedRollBack( &LedGreen );
	}
}

	
void MyKeyTest( u8 key )
{
	switch (key)
	{
		case KEY0:
				LedGreen.LedRollBack( &LedGreen );
				//LedRed.LedBlink( &LedRed, 1000 );
				PwmSetDutyCycle( DutyCycle+10 ); 
			break;
		case KEY1:
				LedGreen.LedRollBack( &LedGreen );
				PwmSetDutyCycle( DutyCycle-10 ); 
			break;
		case KEY2:
				//LedRed.LedOFF( &LedRed );
				PwmSetPeriodUs( Period+50 ); 
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
