#include "i2c.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


static I2C_ATTR i2c_attr;

static void I2cGpioInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//使能 GPIOB 时钟

	GPIO_InitStructure.GPIO_Pin = i2c_attr.PinSDA | i2c_attr.PinSCL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

static void I2cSDAOut( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = i2c_attr.PinSDA; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

static void I2cSDAIn( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = i2c_attr.PinSDA; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
}

s8 I2cInit( I2C_ATTR *pi2cattr )
{
	i2c_attr.PinSDA = pi2cattr->PinSDA;
	i2c_attr.PinSCL = pi2cattr->PinSCL;

	I2cGpioInit(); 

	return 0;

}

static void I2cSDASet( u8 data )
{
	I2cSDAOut( );	
	if( I2CSDAHIGH == data )
		GPIO_SetBits( GPIOB, i2c_attr.PinSDA );
	else if( I2CSDALOW == data )
		GPIO_ResetBits( GPIOB, i2c_attr.PinSDA );
}

static void I2cSCLSet( u8 data )
{
	if( I2CSCLHIGH == data )
		GPIO_SetBits( GPIOB, i2c_attr.PinSCL );
	else if( I2CSCLLOW == data )
		GPIO_ResetBits( GPIOB, i2c_attr.PinSCL );
}

static u8 I2cSDAGet( void )
{
	I2cSDAIn( );	
	return GPIO_ReadInputDataBit( GPIOB, i2c_attr.PinSDA );
}

void I2cStart( void )
{
	I2cSDASet( I2CSDAHIGH );
	I2cSCLSet( I2CSCLHIGH );
	delay_us( 4 );
	I2cSDASet( I2CSDALOW );
	delay_us( 4 );
	I2cSCLSet( I2CSCLLOW );
}

void I2cStop( void )
{
	//I2cSCLSet( I2CSCLLOW );
	I2cSCLSet( I2CSCLHIGH );
	I2cSDASet( I2CSDALOW );
	delay_us( 4 );
	//I2cSCLSet( I2CSCLHIGH );
	I2cSDASet( I2CSDAHIGH );
	delay_us( 4 );
	I2cSCLSet( I2CSCLHIGH );
}

u8 I2cWaitAck( void )
{
	u8 ucErrTime = 0;

	I2cSCLSet( I2CSCLHIGH );
	delay_us( 1 );
	I2cSDASet( I2CSDAHIGH );
	delay_us( 1 );
	
	while( I2cSDAGet() )
	{	
		ucErrTime++;
		if(ucErrTime > 250 )
		{
			I2cStop( );
			return 1;
		}
	}
	I2cSCLSet( I2CSCLLOW );
	return 0;
}

static void I2cAck( void )
{
	I2cSCLSet( I2CSCLLOW );
	I2cSDASet( I2CSDALOW );
	delay_us( 2 );
	I2cSCLSet( I2CSCLHIGH );
	delay_us( 2 );
	I2cSCLSet( I2CSCLLOW );
}

static void I2cNoAck( void )
{
	I2cSCLSet( I2CSCLLOW );
	I2cSDASet( I2CSDAHIGH );
	delay_us( 2 );
	I2cSCLSet( I2CSCLHIGH );
	delay_us( 2 );
	I2cSCLSet( I2CSCLLOW );
}

#if 0
s8 I2cMatch( void )
{

}
#endif

void I2cSendByte( u8 data )
{
	u8 t;

	I2cSCLSet( I2CSCLLOW );
	for( t=0; t<8; t++ )
	{
		I2cSDASet( (data&0X80)>>7 );
		data<<=1;	
		delay_us( 2 );
		I2cSCLSet( I2CSCLHIGH );
		delay_us( 2 );
		I2cSCLSet( I2CSCLLOW );
		delay_us( 2 );
	}
}

u8 I2cRecvByte( u8 ack )
{
	u8 i, receive=0;
	
	for( i=0; i<8; i++ )
	{
		I2cSCLSet( I2CSCLLOW );
		delay_us( 2 );
		I2cSCLSet( I2CSCLHIGH );
		receive <<= 1;
		if( I2cSDAGet() )
			receive++;
		delay_us( 1 );
	}
	
	if( I2CACK == ack )
		I2cAck( );
	else if( I2CNOACK == ack )
		I2cNoAck( );
	
	return receive;
}
