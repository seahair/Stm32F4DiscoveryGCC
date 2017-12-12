#ifndef  __REMOTE_H__
#define  __REMOTE_H__
	
#include "stm32f4xx.h"

#define  REMOTESTAWAIT			0
#define  REMOTESTASTART			1
#define  REMOTESTAREPEAT		2
#define  REMOTESTASTOP			3
#define  REMOTEPINUP			1
#define  REMOTEPINDOWN			0


typedef struct _REMOTE_ATTR{
	u32 value;
	u8 addr;
	u8 cmd;
	u8 id;
	u16 logic0time;
	u16 logic1time;
	u16 starttime;
	u16 stoptime; //10ms
	u16 repeattime;
	u16 timerank;
	u8 state;
	u8 repeatcount;
}REMOTE_ATTR;


void RemoteInit( void );
u8   RemoteRead( void );

#endif 
