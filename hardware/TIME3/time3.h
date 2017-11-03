#ifndef  __TIME3_H__
#define  __TIME3_H__

#include "stm32f4xx.h"

// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us

#define  TIME3PSC		84

void Time3InitMs( u16 time );  //单位 ms
void Time3Start( void );
void Time3Stop( void );





#endif
