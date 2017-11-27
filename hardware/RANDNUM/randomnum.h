#ifndef	 __RANDOMNUM_H__
#define	 __RANDOMNUM_H__

#include "stm32f4xx.h"





typedef enum{
	RNCMDSETRANGE = 0,
	RNCMDSETINT
}RANDNUM_CMD;

typedef struct _RN_ATTR{
	u32 min;
	u32 max;
	u32 value;
}RN_ATTR;

typedef struct _RN_DIR{
	void (*init) ( void );
	u32  (*read) ( void );
	s8   (*ioctrl) ( u8 cmd, RN_ATTR *prnattr );
}RN_DIR;

void RnInit( void );
u32  RnRead( void );
s8   RnIoctrl( u8 cmd, RN_ATTR *prnattr ); 



#endif
