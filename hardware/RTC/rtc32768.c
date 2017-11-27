#include "rtc.h"
#include "usart.h"
#include "rtc32768.h"
#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rtc.h"


static RTC_ATTR rtc_attr;


static void rtc32768_init( void )
{
	rtc_attr.seconds = 0;
	rtc_attr.minutes = 0;
	rtc_attr.hours = 0;
	rtc_attr.date = 28;
	rtc_attr.month = 6;
	rtc_attr.year = 1987;
	rtc_attr.weekday = 7;
	rtc_attr.hourformat = RTC_HourFormat_24;
	rtc_attr.wakeupclk = RTC_WakeUpClock_CK_SPRE_16bits;
	rtc_attr.wakeupcycle = 0;
}

static void rtc32768_read( RTC_ATTR *prtcattr )
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;

	RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStruct);

	prtcattr->seconds = RTC_TimeStruct.RTC_Seconds;
	prtcattr->minutes = RTC_TimeStruct.RTC_Minutes;
	prtcattr->hours   = RTC_TimeStruct.RTC_Hours;
	prtcattr->date	  = RTC_DateStruct.RTC_Date;
	prtcattr->month   = RTC_DateStruct.RTC_Month;
	prtcattr->year	  = RTC_DateStruct.RTC_Year;
	prtcattr->weekday = RTC_DateStruct.RTC_WeekDay;
}

static s8 rtc32768_ioctrl( u8 cmd, RTC_ATTR *prtcattr )
{
	RTC_TimeTypeDef RTC_TimeTypeInitStructure;
	RTC_DateTypeDef RTC_DateTypeInitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmTypeInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;

	switch ( cmd )
	{
		case RTCCMDSETTIME :

			RTC_TimeTypeInitStructure.RTC_Hours = prtcattr->hours;
			RTC_TimeTypeInitStructure.RTC_Minutes = prtcattr->minutes;
			RTC_TimeTypeInitStructure.RTC_Seconds = prtcattr->seconds;
			RTC_TimeTypeInitStructure.RTC_H12 = prtcattr->hourformat;

			RTC_SetTime(RTC_Format_BIN, &RTC_TimeTypeInitStructure);
			break;
		case RTCCMDSETDATE :

			RTC_DateTypeInitStructure.RTC_Date = prtcattr->date;
			RTC_DateTypeInitStructure.RTC_Month = prtcattr->month;
			RTC_DateTypeInitStructure.RTC_WeekDay = prtcattr->weekday;
			RTC_DateTypeInitStructure.RTC_Year = prtcattr->year;

			RTC_SetDate(RTC_Format_BIN,&RTC_DateTypeInitStructure);
			break;
		case RTCCMDSETALARMA :

			RTC_AlarmCmd(RTC_Alarm_A,DISABLE);//关闭闹钟A 

			RTC_TimeTypeInitStructure.RTC_Hours = prtcattr->hours;//小时
			RTC_TimeTypeInitStructure.RTC_Minutes = prtcattr->minutes;//分钟
			RTC_TimeTypeInitStructure.RTC_Seconds = prtcattr->seconds;//秒
			RTC_TimeTypeInitStructure.RTC_H12 = RTC_H12_AM;

			RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDay = prtcattr->weekday;//星期
			RTC_AlarmTypeInitStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_WeekDay;//按星期闹
			RTC_AlarmTypeInitStructure.RTC_AlarmMask=RTC_AlarmMask_None;//精确匹配星期，时分秒
			RTC_AlarmTypeInitStructure.RTC_AlarmTime=RTC_TimeTypeInitStructure;
			RTC_SetAlarm(RTC_Format_BIN,RTC_Alarm_A,&RTC_AlarmTypeInitStructure);

			RTC_ClearITPendingBit(RTC_IT_ALRA);//清除RTC闹钟A的标志
			EXTI_ClearITPendingBit(EXTI_Line17);//清除LINE17上的中断标志位 

			RTC_ITConfig(RTC_IT_ALRA,ENABLE);//开启闹钟A中断
			RTC_AlarmCmd(RTC_Alarm_A,ENABLE);//开启闹钟A 
			EXTI_InitStructure.EXTI_Line = EXTI_Line17;//LINE17
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE17
			EXTI_Init(&EXTI_InitStructure);//配置

			NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; 
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
			NVIC_Init(&NVIC_InitStructure);//配置
			break;
		case RTCCMDENABLEALARMA :
			RTC_AlarmCmd(RTC_Alarm_A, ENABLE);//开启闹钟A 
			break;
		case RTCCMDDISABLEALARMA :
			RTC_AlarmCmd(RTC_Alarm_A, DISABLE);//关闭闹钟A 
			break;
		case RTCCMDSETALARMB :
			break;
		case RTCCMDENABLEALARMB :
			break;
		case RTCCMDDISABLEALARMB :
			break;
		case RTCCMDSETWAKEUP :

			RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
			RTC_WakeUpClockConfig( prtcattr->wakeupclk );//唤醒时钟选择
			RTC_SetWakeUpCounter( prtcattr->wakeupcycle );//设置WAKE UP自动重装载寄存器

			RTC_ClearITPendingBit(RTC_IT_WUT); //清除RTC WAKE UP的标志
			EXTI_ClearITPendingBit(EXTI_Line22);//清除LINE22上的中断标志位 
			RTC_ITConfig(RTC_IT_WUT,ENABLE);//开启WAKE UP 定时器中断
			RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器　

			EXTI_InitStructure.EXTI_Line = EXTI_Line22;//LINE22
			EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
			EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
			EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE22
			EXTI_Init(&EXTI_InitStructure);//配置

			NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn; 
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;//抢占优先级1
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
			NVIC_Init(&NVIC_InitStructure);//配置
			break;
		case RTCCMDENABLEWAKEUP :
			RTC_WakeUpCmd( ENABLE);//开启WAKE UP 定时器
			break;
		case RTCCMDDISABLEWAKEUP :
			RTC_WakeUpCmd(DISABLE);//关闭WAKE UP
			break;
		default:
			return 0;
	}
}


const RTC_DIR rtc32768_module = {
	rtc32768_init,
	rtc32768_read,
	rtc32768_ioctrl
};



