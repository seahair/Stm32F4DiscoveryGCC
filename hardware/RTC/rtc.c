#include "rtc.h"
#include "rtc32768.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rtc.h"

#define  ARRAYLEN(array)	(sizeof(array)/sizeof(array[0]))

const RTC_DIR *rtc_dir = 0;


const RTC_DIR * RtcModules[] = {
	&rtc32768_module,

};



static RtcClockInit( void )
{
	RTC_InitTypeDef RTC_InitStructure;
	u16 retry=0X1FFF; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能PWR时钟
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问

	if(RTC_ReadBackupRegister(RTC_BKP_DR0)!=0x5050)		//是否第一次配置?
	{
		RCC_LSEConfig(RCC_LSE_ON);//LSE 开启    
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
			retry++;
			delay_ms(10);
		}
		if(retry==0)return 1;		//LSE 开启失败. 

		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟 

		RTC_InitStructure.RTC_AsynchPrediv = 0x7F;//RTC异步分频系数(1~0X7F)
		RTC_InitStructure.RTC_SynchPrediv  = 0xFF;//RTC同步分频系数(0~7FFF)
		RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;//RTC设置为,24小时格式
		RTC_Init(&RTC_InitStructure);

		RTC_WriteBackupRegister(RTC_BKP_DR0,0x5050);	//标记已经初始化过了
	} 

	return 0;
}

static s8 RtcMatch( void )
{
	u8 i=0;

	for( i=0; i<ARRAYLEN(RtcModules); i++ )
	{
		rtc_dir = RtcModules[i];		
	}

	return 0;
}

void RtcInit( void )
{
	RtcClockInit( );

	if( 0 == RtcMatch() )
	{
		rtc_dir->init( );
	}
}

s8 RtcIoctrl( u8 cmd, RTC_ATTR *prtcattr )
{
	if( rtc_dir )
		rtc_dir->ioctrl( cmd, prtcattr );
}

void RtcRead( RTC_ATTR *prtcattr )
{
	if( rtc_dir )
		rtc_dir->read( prtcattr );
}


