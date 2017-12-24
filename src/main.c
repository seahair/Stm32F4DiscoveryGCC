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
#include "randomnum.h"
#include "pm.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "at24cxx.h"
#include "spi.h"
#include "w25qxx.h"
#include "remote.h"
#include "touch.h"
#include "flash.h"
#include "sram.h"
#include "malloc.h"



#define  SRAMNUM  100
u8 sramtest[SRAMNUM] __attribute__((section(".glx")));

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

u16 DacCount= 10;

#if 0
#define DMATXNUM 8200
u8 dmatxbuf[DMATXNUM];
#endif

#define  EEPROMLEN 12
u8  eepromw[EEPROMLEN] = "I LOVE YOU\r\n";
u8  eepromr[EEPROMLEN];
void MyKeyTest( u8 key );

const u8 TEXT_Buffer[]={"I LOVE YOU FLASH TEST"};
#define TEXT_LENTH sizeof(TEXT_Buffer) //数组长度
#define SIZE TEXT_LENTH/4+((TEXT_LENTH%4)?1:0)
#define FLASH_SAVE_ADDR 0X0802C004 


int main(int argc, char *argv[])
{
	u16 x = 0;
	u16 y = 790;
	s8 lcd_id[12];
	u8 rtcbuf[40];
	float value; 

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
	LcdIoctl( LCDCMDSETSHOWMODE, LCDMODEADDBACK );

	RTC_ATTR myrtc;	
	myrtc.year = 17;
	myrtc.month = 11;
	myrtc.date = 27;
	myrtc.hours = 22;
	myrtc.minutes = 10;
	myrtc.seconds = 0;
	myrtc.weekday = 1;
	myrtc.hourformat = RTC_HourFormat_24;

	//RtcIoctrl( RTCCMDSETTIME, &myrtc );
	//RtcIoctrl( RTCCMDSETDATE, &myrtc ); 

	//RN_ATTR myrn = { 50, 199, 0 };
	//RnIoctrl( RNCMDSETRANGE, &myrn );

#if 0
	u16 i = 0;
	for( i=0; i<DMATXNUM-2; i++ )
		dmatxbuf[i] = i;
	dmatxbuf[DMATXNUM-2] = '\r';
	dmatxbuf[DMATXNUM-1] = '\n';
	DMA_ATTR mydmaattr;
	mydmaattr.memoryaddr = &dmatxbuf; 
	mydmaattr.txsize = DMATXNUM;
	DmaInit( DMAUSART1TX, &mydmaattr );
	DmaIoCtrl( DMACMDSETMEMADDR, &mydmaattr );
	DmaIoCtrl( DMACMDSETTXSIZE, &mydmaattr );
	DmaIoCtrl( DMACMDSTART, &mydmaattr ); 
#endif

	//at24cxx_write( 0X10, EEPROMLEN, eepromw );

	//const u8 TEXT_Buffer[]={"Explorer STM32F4 SPI TEST"};
	//#define SIZE sizeof(TEXT_Buffer)
	//	W25QXX_Write((u8*)TEXT_Buffer,200,SIZE);

#if 0
	//FlashWrite(FLASH_SAVE_ADDR,(u32*)TEXT_Buffer,SIZE);

	u8 datatemp[SIZE];
	MYFLASH *myflash = FlashCreat( );
	myflash->write( myflash, FLASH_SAVE_ADDR,(u32*)TEXT_Buffer,SIZE); 
	myflash->read( myflash, FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);
	LcdShowString(30,190,datatemp);
#endif

#if 1

	MYMALLOC *mymalloc = MallocCreat( );
	u8 *psize1 = mymalloc->malloc( mymalloc, 32100 );
	u8 *psize2 = mymalloc->malloc( mymalloc, 32100 );
	u8 *psize3 = mymalloc->malloc( mymalloc, 32100 );
	
#endif


	while(1)
	{
		delay_ms(1000);
		LedRed.LedRollBack( &LedRed );

		LcdShowString( 600, 20, "Hello Linus" );
		RtcRead( &myrtc );			
		sprintf((char*)rtcbuf,"Date:%02d.%02d.%02d",myrtc.year,myrtc.month,myrtc.date);
		LcdShowString( 20, 20, rtcbuf );
		sprintf((char*)rtcbuf,"Time:%02d:%02d:%02d",myrtc.hours,myrtc.minutes,myrtc.seconds);
		LcdShowString( 210, 20, rtcbuf );
		sprintf((char*)rtcbuf,"Weekday:%02d", myrtc.weekday);
		LcdShowString( 400, 20, rtcbuf );


		printf("psize1 malloc test address is 0x%x \r\n", psize1 );
		printf("psize2 malloc test address is 0x%x \r\n", psize2 );
		printf("psize3 malloc test address is 0x%x \r\n", psize3 );
		sprintf( (char*)rtcbuf, "exti sram preuse:%2d", mymalloc->preuse(mymalloc));	
		LcdShowString( 20, 200, rtcbuf );

#if 0
		for( u16 i=0; i<SRAMNUM; i++ )
		{
			sramtest[i] = i;
			printf(" sramtest address is 0x%x \r\n", &sramtest[i] );
		}
#endif 

#if 0
		u8 datatemp[SIZE];

		FlashRead(FLASH_SAVE_ADDR,(u32*)datatemp,SIZE);
		LcdShowString(30,190,datatemp);
#endif

#if 0
		TOUCH_ATTR mytoucattr;	
		mytoucattr.dir = TOUCHDIVH;
		while( 1 )
		{
			TouchGetxy( &mytoucattr ); 
			for( u8 i=0; i<mytoucattr.touchnum; i++ )
			{
				for( u8 j=1; j<3; j++ )
				{
					LcdDrawPixel( mytoucattr.LCDXY.x[i], mytoucattr.LCDXY.y[i], RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i]-j, mytoucattr.LCDXY.y[i], RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i]+j, mytoucattr.LCDXY.y[i], RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i], mytoucattr.LCDXY.y[i]+j, RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i], mytoucattr.LCDXY.y[i]-j, RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i]+j, mytoucattr.LCDXY.y[i]+j, RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i]+j, mytoucattr.LCDXY.y[i]-j, RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i]-j, mytoucattr.LCDXY.y[i]+j, RED );
					LcdDrawPixel( mytoucattr.LCDXY.x[i]-j, mytoucattr.LCDXY.y[i]-j, RED );
				}
			}
			delay_ms(10);
		}
#endif

#if 0
		u8 cmd = RemoteRead( );
		sprintf((char*)rtcbuf,"Recv Remote Cmd:%02d", cmd);
		LcdShowString( 20, 200, rtcbuf );
#endif

#if 0
		if(W25QXX_ReadID()!=W25Q128)
		{
			LcdShowString(20, 120, "W25Q128 Check Failed!");
			delay_ms(500);
			LcdShowString(20, 240, "Please Check!      ");
			delay_ms(500);
		}

		LcdShowString(30,170,"Start Read W25Q128.... ");
		u8 datatemp[SIZE];
		W25QXX_Read(datatemp,200,SIZE);
		LcdShowString(30,170,"The Data Readed Is:   ");	
		LcdShowString(30,190,datatemp);
#endif

#if 0	
		at24cxx_read( 0X10, EEPROMLEN, eepromr );
		LcdShowString( 20, 120, eepromr );
		LcdShowString( 20, 220, eepromw );
#endif

#if 0
		sprintf( (char*)rtcbuf,"DMA Status: %2d", DmaGetStatus( ) );
		LcdShowString( 20, 120, rtcbuf );

		sprintf( (char*)rtcbuf,"DMA Transprogess: %.2f", DmaGetTransProgess( ) );
		LcdShowString( 20, 240, rtcbuf );
#endif

#if 0
		DacCount += 10;
		DacWrite( DacCount );
#endif

#if 0
		value = AdcRead( );
		sprintf( (char*)rtcbuf, "ADC Value:%.4f", value );
		LcdShowString( 20, 170, rtcbuf );
#endif

#if 0
		u32 rdnum = RnRead( );
		sprintf((char*)rtcbuf,"RandomNum:%04d", rdnum);
		LcdShowString( 20, 170, rtcbuf );
#endif		

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

#if 0
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
#endif

void EXTI2_IRQHandler(void)
{ 
	delay_ms(10); //消抖
	if( KEY2PRESS == GetKeyStatus( KEY2 ) )
	{
		LedGreen.LedRollBack( &LedGreen );
		//PwmSetDutyCycle( DutyCycle-=10 ); 
		DacCount++;
	}
	EXTI_ClearITPendingBit(EXTI_Line2); //清除 LINE2 上的中断标志位
}

void EXTI3_IRQHandler(void)
{ 
	delay_ms(10); //消抖
	if( KEY1PRESS == GetKeyStatus( KEY1 ) )
	{
		LedGreen.LedRollBack( &LedGreen );
		//PwmSetPeriodUs( Period+=50 ); 
	}
	EXTI_ClearITPendingBit(EXTI_Line3); //清除 LINE3 上的中断标志位
}

void EXTI4_IRQHandler(void)
{ 
	delay_ms(10); //消抖
	if( KEY0PRESS == GetKeyStatus( KEY0 ) )
	{
		LedGreen.LedRollBack( &LedGreen );
		//PwmSetPeriodUs( Period-=50 ); 
		DacCount--;
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

//中断,检测到PA0脚的一个上升沿.	  
//中断线0线上的中断检测
void EXTI0_IRQHandler(void)
{													    
	EXTI_ClearITPendingBit(EXTI_Line0); // 清除LINE10上的中断标志位
	if(PmRead())//关机?
	{		  
		PM_ATTR mypmattr;
		PmIoctrl( PMCMDSTART, &mypmattr ); 
	}
} 

