#ifndef	 __IWDG_H__
#define  __IWDG_H__

#include "stm32f4xx.h"

#define  PRER1S		4
#define  RLR1S		500



void IwdgStart1S( void );
void IwdgFeed( void );





#endif
