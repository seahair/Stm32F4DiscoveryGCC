#ifndef  __PWM_H__
#define	 __PWM_H__

#include "stm32f4xx.h"



#define  PWMHZ			84
#define  PWMPERIOD		500

//void PwmTimeInit1Us( void );		//default counter 1us, period 500us
void PwmInit( void );
void PwmSetPeriodUs( u16 time );	// us
void PwmSetDutyCycle( u16 num );	// num < time 
void PwmStart( void );
void PwmStop( void );





#endif
