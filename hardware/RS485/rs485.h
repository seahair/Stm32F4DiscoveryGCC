#ifndef  __RS485_H__
#define  __RS485_H__

#include "stm32f4xx.h"

#define  RS485TXEN			1
#define  RS485RXEN  		0



typedef struct _RS485_ATTR{
	u16 PinA;
	u16 PinB;
	u16 PinEn;
	u32 boundrate;
}RS485_ATTR;


s8 Rs485Init( RS485_ATTR *prs485attr );
void Rs485Send( u8* addr, u16 len );
u16 Rs485Recv( u8* addr, u16 len );


#endif
