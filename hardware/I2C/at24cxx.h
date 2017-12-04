#ifndef  __AT24CXX_H__
#define  __AT24CXX_H__ 

#include "i2c.h"
#include "stm32f4xx.h"

#define AT24C02FLAG			0X55

//s8 at24cxx_match( void );
s8 at24cxx_init( void );
void at24cxx_write( u16 WriteAddr, u16 len, u8 *pbuf );
void at24cxx_read( u16 ReadAddr, u16 len, u8 *pbuf );


#endif 
