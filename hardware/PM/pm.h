#ifndef  __PM_H__
#define  __PM_H__

#include "stm32f4xx.h"

typedef enum {
	PMCMDENABLE = 0,
	PMCMDDISABLE,
	PMCMDSTART,
	PMCMDSTOP,
	PMCMDSETHOLDTIME
}PM_CMD;

typedef struct _PM_ATTR{
	u16 holdtime; //ms
}PM_ATTR;

typedef struct _PM_DIR{
	s8 (*init) ( void );
	void (*ioctrl) ( u8 cmd, PM_ATTR *ppmattr );
	s8 (*read) ( void );
}PM_DIR;


s8 PmInit( void );
void PmIoctrl( u8 cmd, PM_ATTR *ppmattr );
s8  PmRead( void );


#endif

