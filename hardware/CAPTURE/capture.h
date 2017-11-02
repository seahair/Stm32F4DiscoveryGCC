#ifndef	  __CAPTURE_H__
#define   __CAPTURE_H__

#include  "stm32f4xx.h"

#define  CAPTUREHZ			84
#define  CAPTURENUM			0XFFFFFFFF	
#define  CAPTUREFINSH		0X00
#define  CAPTURESTART		0X01
#define  CAPTUREWAIT		0x02
#define  CAPTURETIMEOUT		0X03

#if 0
#define  CAPTURESTATUS	0X80
#define  CAPTUREDOING	0X40
#define  CAPTUREOCNUM	0X3F
#define  CAPTUREFINSH	0X80
#define  CAPTURESTART	0X00
#endif


extern u32 CaptureCount;			//用于记录 溢出的 次数
extern u8  CaptureStatus;			//用于捕获 状态机
extern uint64_t CaptureTime;		//用于记录 最终结果，捕获的时间 单位us


void CapturePinInit( void );
void CaptureTimeInit1us( u32 per );
void CaptureInterruptInit( void );
void CaptureInit( void );
void CaptureStart( void );
u32  CaptureGetValue( void );
void CaptureStop( void );
uint64_t CaptureClacTime( u32 num );



#endif
