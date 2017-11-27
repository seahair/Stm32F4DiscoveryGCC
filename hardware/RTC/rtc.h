#ifndef	 __RTC_H__
#define	 __RTC_H__

#include "stm32f4xx.h"


typedef enum{
	RTCCMDSETTIME = 0,
	RTCCMDSETDATE,
	RTCCMDSETALARMA,
	RTCCMDENABLEALARMA,
	RTCCMDDISABLEALARMA,
	RTCCMDSETALARMB,
	RTCCMDENABLEALARMB,
	RTCCMDDISABLEALARMB,
	RTCCMDSETWAKEUP,
	RTCCMDENABLEWAKEUP,
	RTCCMDDISABLEWAKEUP
}RTC_CMD;


typedef struct _RTC_ATTR{
	u8 seconds;
	u8 minutes;
	u8 hours;
	u8 date;
	u8 month;
	u16 year;
	u8 weekday;
	u32 hourformat;
	u32 wakeupclk;
	u16 wakeupcycle;
}RTC_ATTR;

typedef struct _RTC_DIR{
	void (*init) ( void );
	void (*read) ( RTC_ATTR *prtcattr );
	s8   (*ioctrl) ( u8 cmd, RTC_ATTR *prtcattr );
}RTC_DIR;


void  RtcInit ( void );
void  RtcRead ( RTC_ATTR *prtcattr );
s8	  RtcIoctrl ( u8 cmd, RTC_ATTR *prtcattr );


//const extern RTC_DIR *rtc_dir ;


#endif
