#include "i2c.h"
#include "delay.h"
#include "at24cxx.h"
#include "stm32f4xx_gpio.h"

const I2C_ATTR at24c02_attr = {
	GPIO_Pin_9,
	GPIO_Pin_8,
	GPIOB,
	GPIOB,
	0X2402,
	0XFF
};


static u8 At24cxx_readByte( u16 Addr )
{
	u8 temp=0;
	
	I2cStart( );
	I2cSendByte( 0XA0+ ((Addr/256)<<1) );	//发送　写　命令
	I2cWaitAck( );
	I2cSendByte( Addr%256 );
	I2cWaitAck( );
	I2cStart( );
	I2cSendByte( 0XA1 );	//　发送　读　命令
	I2cWaitAck( );
	temp = I2cRecvByte(	I2CNOACK );
	I2cStop( );

	return temp;
}

static void At24cxx_writeByte( u16 Addr, u8 data )
{
	I2cStart( );
	I2cSendByte( 0XA0+ ((Addr/256)<<1) );	//发送　写　命令
	I2cWaitAck( );
	I2cSendByte( Addr%256 );
	I2cWaitAck( );
	I2cSendByte( data );
	I2cWaitAck( );
	I2cStop( );
	delay_ms( 10 );
}

void at24cxx_write( u16 WriteAddr, u16 len, u8 *pbuf )
{
	if( WriteAddr<at24c02_attr.size )
	{
		while( len-- )
		{
			At24cxx_writeByte( WriteAddr++, *(pbuf++) );
		}
	}
	
}

void at24cxx_read( u16 ReadAddr, u16 len, u8 *pbuf )
{
	if( ReadAddr<at24c02_attr.size )
	{
		while( len-- )
		{
			*(pbuf++) = At24cxx_readByte( ReadAddr++ );
		}
	}	
}

static s8 at24cxx_match( void )
{
	u8 temp;

	temp = At24cxx_readByte( at24c02_attr.size );
	if( AT24C02FLAG == temp )
		return 0;
	else 
	{
		At24cxx_writeByte( at24c02_attr.size, AT24C02FLAG );
		temp = At24cxx_readByte( at24c02_attr.size );
		if( AT24C02FLAG == temp )
			return 0;
	}

	return -1;
}

s8 at24cxx_init( void )
{
	I2cInit( &at24c02_attr );	
	
	if( at24cxx_match() == 0 )
		return 0;
	else
		return -1;
}

