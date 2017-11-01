#ifndef  __KEY_H__	
#define  __KEY_H__

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"


enum _KEY{
	KEY0,
	KEY1,
	KEY2,
	KEYWK,
	_KEYNUM
};
#define  KEYMAXNUM   _KEYNUM 

#define  PINKEY0	 GPIO_Pin_4
#define  PINKEY1	 GPIO_Pin_3
#define  PINKEY2	 GPIO_Pin_2	
#define  PINKEYWK	 GPIO_Pin_0

#define  KEY0UNPRESS		1
#define	 KEY1UNPRESS		1
#define  KEY2UNPRESS		1
#define  KEYWKUNPRESS		0

#define	 KEY0PRESS			0
#define  KEY1PRESS			0
#define  KEY2PRESS			0
#define  KEYWKPRESS			1


typedef void (* KEYTEST) ( u8 );


void KeyPinInit( void );
s8   GetKeyStatus( u8 pin );
u8   WhichKeyPress( void );
void KeyTest( KEYTEST pFun );
void KeyExtiInit( void );





#endif
