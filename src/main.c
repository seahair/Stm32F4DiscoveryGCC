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
#include "tpad.h"
#include "lcd.h"
#include "rtc.h"
/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "usart.h"
#include <stdio.h>
#include <stdlib.h>
#include "sram.h"*/

u16 DutyCycle = 100;
u16 Period = 500;
u8  CaptureStatus = 0; 
u32 CaptureTime = 0;
u32 CaptureCount  = 0;

u8  TPADStatus = 0; 
u32 TPADTime = 0;
u32 TPADDefaultTime = 0;

//u32 Guolinxin=0;
//_lcd_dev lcddev ;
//u16 POINT_COLOR=0x0000;	//»­±ÊÑÕÉ«
//u16 BACK_COLOR=0xFFFF;  //±³¾°É«

void MyKeyTest( u8 key );


int main(int argc, char *argv[])
{
	u16 x = 0;
	u16 y = 790;
	s8 lcd_id[12];
	u8 rtcbuf[40];

	HardInit( );
	//PwmStart( );
	//Time3Start( );
	//CaptureStart( );    
	//CaptureStatus = CAPTURESTART;
	//TPADDefaultTime = TpadTestDefaultTime( 8 );	


	LcdIoctl( LCDCMDSETASC2SIZE, ASC2_24 );
	LcdIoctl( LCDCMDSETBRUSHCOLOR, RED );
	LcdIoctl( LCDCMDSETBACKCOLOR, YELLOW );
	LcdIoctl( LCDCMDSETSHOWMODE, LCDMODENOBACK );
	LcdIoctl( LCDCMDSETDIR, D2U_L2R );

	RTC_ATTR myrtc;	

	while(1)
	{
		delay_ms(1000);
		LedRed.LedRollBack( &LedRed );

#if 1
		x++;
		y--;
		sprintf((char*)lcd_id,"LcdID:%04X", x);
		//LcdDrawLine( x, y, 480-x, 800-y );
		LcdShowString( 20, 20, lcd_id );
		LcdShowString( 400, 240, "Hello Linus" );
		LcdIoctl( LCDCMDSETSHOWMODE, LCDMODEADDBACK );
#endif

		RtcRead( &myrtc );			
		sprintf((char*)rtcbuf,"Time:%02d:%02d:%02d",myrtc.hours,myrtc.minutes,myrtc.seconds);
		LcdShowString( 20, 400, rtcbuf );

#if 0
		TPADTime = TpadGetCapTime( );
		if( TPADTime > TPADDefaultTime+TPADTHRESHOLD )	
		{
			TPADStatus++ ;
		} 
		if( TPADStatus> 7 )
		{
			printf("TPADDefaultTime is %d \r\n",TPADDefaultTime );
			printf("TPADTime is %d \r\n",TPADTime );
			LedGreen.LedRollBack( &LedGreen );
			TPADStatus= 0;
		}
#endif 

		//TIM_SetCompare1(TIM14,led0pwmval);

		/* if( CaptureStatus==CAPTUREFINSH || CaptureStatus==CAPTURETIMEOUT )
		   {
		   printf("Time is %d us \r\n", CaptureTime );
		   CaptureStatus = CAPTURESTART;
		   CaptureTime = 0;
		   }*/
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

	//LedRed.LedRollBack( &LedRed );
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_Update);
}


#if 0
void TIM2_IRQHandler(void)
{
	LedRed.LedRollBack( &LedRed );
	switch ( TPADStatus )
	{
		case TPADSTUSSTART:
			if(TIM_GetITStatus(TIM2, TIM_IT_CC1)==SET) //捕获中断
			{
				TPADTime = TpadGetCapTime( );
				TIM_SetCounter(TIM2, 0);
				//TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);
				TPADStatus = TPADSTUSWAIT;
				//TpadInterruptStart( );
			}
			break;
		case TPADSTUSWAIT:
			if(TIM_GetITStatus(TIM2, TIM_IT_CC1)==SET) //捕获中断
			{
				TPADTime = TpadGetCapTime( );
				TIM_SetCounter(TIM2, 0);
				TIM_OC1PolarityConfig(TIM2, TIM_ICPolarity_Rising);
				TPADStatus = TPADSTUSFINISH;
			}
			break;
		case TPADSTUSFINISH:
			break;
	}
}
#endif


//RTC闹钟中断服务函数
void RTC_Alarm_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF)==SET)//ALARM A中断?
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//清除中断标志
		printf("ALARM A!\r\n");
	}   
	EXTI_ClearITPendingBit(EXTI_Line17);	//清除中断线17的中断标志											 
}


//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF)==SET)//WK_UP中断?
	{ 
		RTC_ClearFlag(RTC_FLAG_WUTF);	//清除中断标志
		LedRed.LedRollBack( &LedRed );
	}   
	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志								
}

