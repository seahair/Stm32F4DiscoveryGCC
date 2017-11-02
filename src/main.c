#include "stm32f4xx.h"
#include "hardwareinit.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "key.h"
#include "time3.h"
#include "pwm.h"
#include "capture.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "pad.h"
#include "sram.h"*/

u16 DutyCycle = 100;
u16 Period = 500;
u8  CaptureStatus = 0; 
uint64_t CaptureTime = 0;
u32 CaptureCount  = 0;

void MyKeyTest( u8 key );

int main(int argc, char *argv[])
{
        HardInit( );
        //PwmStart( );
        //Time3Start( );
        CaptureStart( );    
        CaptureStatus = CAPTURESTART;
        while(1)
        {
                delay_ms(1000);
                //KeyTest( MyKeyTest );	
                LedGreen.LedRollBack( &LedGreen );
                //LedRed.LedRollBack( &LedRed );
                //TIM_SetCompare1(TIM14,led0pwmval);
                if( CaptureStatus==CAPTUREFINSH || CaptureStatus==CAPTURETIMEOUT )
                {
                    printf("Time is %u us \r\n", CaptureTime );
                    CaptureStatus = CAPTURESTART;
                    CaptureTime = 0;
                }
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

void EXTI0_IRQHandler(void)
{ 
        delay_ms(10); //消抖
        if( KEYWKPRESS == GetKeyStatus( KEYWK ) )
        {
                LedGreen.LedRollBack( &LedGreen );
                PwmSetDutyCycle( DutyCycle+=10 ); 
        }
        EXTI_ClearITPendingBit(EXTI_Line0); //清除 LINE0 上的中断标志位
}

void EXTI2_IRQHandler(void)
{ 
        delay_ms(10); //消抖
        if( KEY2PRESS == GetKeyStatus( KEY2 ) )
        {
                LedGreen.LedRollBack( &LedGreen );
                PwmSetDutyCycle( DutyCycle-=10 ); 
        }
        EXTI_ClearITPendingBit(EXTI_Line2); //清除 LINE2 上的中断标志位
}

void EXTI3_IRQHandler(void)
{ 
        delay_ms(10); //消抖
        if( KEY1PRESS == GetKeyStatus( KEY1 ) )
        {
                LedGreen.LedRollBack( &LedGreen );
                PwmSetPeriodUs( Period+=50 ); 
        }
        EXTI_ClearITPendingBit(EXTI_Line3); //清除 LINE3 上的中断标志位
}

void EXTI4_IRQHandler(void)
{ 
        delay_ms(10); //消抖
        if( KEY0PRESS == GetKeyStatus( KEY0 ) )
        {
                LedGreen.LedRollBack( &LedGreen );
                PwmSetPeriodUs( Period-=50 ); 
        }
        EXTI_ClearITPendingBit(EXTI_Line4); //清除 LINE4 上的中断标志位
}

#if 0
void TIM5_IRQHandler(void)
{
        if( CAPTURESTART == (CaptureStatus&CAPTURESTATUS) )
        {
                if(TIM_GetITStatus(TIM5, TIM_IT_Update)==SET) //溢出中断
                {
                        if( CaptureStatus&CAPTUREDOING )
                        {
                                if( (CaptureStatus & CAPTUREOCNUM) < 64)
                                {
                                        CaptureStatus ++;
                                }
                                else
                                {
                                        CaptureCount = CaptureClacTime( CaptureGetValue() );
                                        CaptureStatus = 0;
                                        CaptureStatus |= CAPTUREFINSH;
                                        TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
                                }
                        }
                }

                if(TIM_GetITStatus(TIM5, TIM_IT_CC1)==SET) //捕获中断
                {
                        if( CaptureStatus&CAPTUREDOING )
                        {
                                CaptureCount = CaptureClacTime( CaptureGetValue() );
                                CaptureStatus = 0;
                                CaptureStatus |= CAPTUREFINSH;
                                TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
                        }
                        else
                        {
                                CaptureStatus = 0;
                                CaptureCount  = 0;
                                CaptureStatus |= CAPTUREDOING;
                                TIM_SetCounter(TIM5,0);
                                TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);
                                CaptureStart( );
                        }
                }

        }

                LedRed.LedRollBack( &LedRed );
        TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update);
}
#endif


void TIM5_IRQHandler(void)
{
    u32 temp;
	switch ( CaptureStatus )	
	{	
		case CAPTURESTART :
            if(TIM_GetITStatus(TIM5, TIM_IT_CC1)==SET) //捕获中断
			{
				TIM_SetCounter(TIM5,0);
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);
				CaptureCount = 0;
				CaptureStatus = CAPTUREWAIT;
				CaptureStart( );
			}
			break;
		case CAPTUREWAIT :
            if(TIM_GetITStatus(TIM5, TIM_IT_CC1)==SET) //捕获中断
			{
				temp =  CaptureGetValue();
				CaptureTime = CaptureClacTime( temp );
				//CaptureTime = 6789; 
                TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
				CaptureStatus = CAPTUREFINSH;
				CaptureCount = 0;
			}

            if(TIM_GetITStatus(TIM5, TIM_IT_Update)==SET) //溢出中断
			{
				if( CaptureCount > 20 )
				{
					CaptureTime = CaptureClacTime( CaptureGetValue() );
					TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising);
					CaptureStatus = CAPTURETIMEOUT;
					CaptureCount = 0;
				}
				else
					CaptureCount++ ;
			}
			
			break;
		case CAPTUREFINSH :
		case CAPTURETIMEOUT :
			break;
		default:
			break;
	}

    LedRed.LedRollBack( &LedRed );
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update);
}

