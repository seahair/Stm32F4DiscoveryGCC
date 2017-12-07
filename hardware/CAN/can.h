#ifndef  __CAN_H__
#define  __CAN_H__

#include "stm32f4xx.h"

#define  CANMODELOOPBACK		1
#define  CANMODENORMAL			0

typedef enum {
	CANENABLE,
	CANDISABLE,
	CANSETMODE
}CAN_CMD;


typedef struct _CAN_ATTR{
	u16 CanPinH;
	u16 CanPinL;
	u8  mode;
	u8  sjwtime;
	u8  bs1time;
	u8  bs2time;
	u16  prescaler;
}CAN_ATTR;


s8 CanInit( CAN_ATTR *pcanattr );
void CanIoctrl( u8 cmd, CAN_ATTR *pcanattr );
s8 CanSend( u8 *buf, u16 len );
u8 CanRecv( u8 *buf );



#endif
