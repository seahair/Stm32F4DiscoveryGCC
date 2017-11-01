#ifndef	  __CAPTURE_H__
#define   __CAPTURE_H__

#include  "stm32f4xx.h"

#define  CAPTUREHZ		84
#define  CAPTURENUM		0XFFFFFFFF	
#define  CAPTURESTATUS	0X80
#define  CAPTUREDOING	0X40
#define  CAPTUREOCNUM	0X3F
#define  CAPTUREFINSH	0X80
#define  CAPTURESTART	0X00



extern u32 CaptureCount;
extern u8  CaptureStatus;


void CapturePinInit( void );
void CaptureTimeInit1us( u32 per );
void CaptureInterruptInit( void );
void CaptureInit( void );
void CaptureStart( void );
u32  CaptureGetValue( void );
void CaptureStop( void );
uint64_t  CaptureClacTime( u32 num );



#endif
