#include "randomnum.h"
#include "rng.h"
#include "delay.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"

#define  ARRAYLEN(array)	(sizeof(array)/sizeof(array[0]))

const RN_DIR *rn_dir=0;

static RN_DIR *rn_modules[] = {
	&rng_module,
};	



static s8 RnClockInit( void )
{
	u16 retry=0; 
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE); //开启 RNG 时钟

	RNG_Cmd(ENABLE); //使能 RNG
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET&&retry<10000)//等待就绪
	{
		retry++; 
		delay_us(100);
	}

	if(retry>=10000)
		return -1;//随机数产生器工作不正常

	return 0;
}

static s8 RnMatch( void )
{
	u8 i=0;

	for( i=0; i<ARRAYLEN(rn_modules); i++ )
	{
		rn_dir = rn_modules[i];
	}
	
	return 0;
}

void RnInit( void )
{
	s8 ret=0;
	ret = RnClockInit( );
	
	if( ret != 0 )
		printf("Random Number init error!! \r\n");

	if( 0 == RnMatch( ) )
	{
		rn_dir->init( );
	}
}

u32 RnRead( void )
{
	if( rn_dir )
		rn_dir->read( );
}

s8 RnIoctrl( u8 cmd, RN_ATTR *prnattr )
{
	if( rn_dir )
		rn_dir->ioctrl( cmd, prnattr );
}

