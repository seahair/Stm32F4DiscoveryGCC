#ifndef  __I2C_H__
#define  __I2C_H__ 

#include "stm32f4xx.h"

#define  I2CSDAHIGH			1
#define  I2CSDALOW			0
#define  I2CSCLHIGH			1
#define  I2CSCLLOW			0
#define  I2CACK				1
#define  I2CNOACK			0




typedef struct _I2C_ATTR{
	u16  PinSDA;
	u16  PinSCL;
	GPIO_TypeDef* TypeSDA;
	GPIO_TypeDef* TypeSCL;
	u16  name;
	u16  size;
}I2C_ATTR;

#if 0
typedef struct _I2C_DIR{
	s8 (*init) ( void );
	s8 (*match) ( void );
	
}I2C_DIR;
#endif 

//s8 I2cMatch( void );
s8 I2cInit( I2C_ATTR *pi2cattr );
void I2cStart( void );
void I2cStop( void );
u8 I2cWaitAck( void );
void I2cSendByte( u8 data );
u8   I2cRecvByte( u8 ack );



#endif

