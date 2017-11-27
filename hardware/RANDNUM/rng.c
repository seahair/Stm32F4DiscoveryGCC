#include "rng.h"
#include "randomnum.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rng.h"

static RN_ATTR rn_attr;

static void rng_init( void )
{
	rn_attr.min = 0;
	rn_attr.max = 99;
	rn_attr.value = 0;
}

static u32 rng_getrandomnum( void )
{
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET);	//µÈŽýËæ»úÊýŸÍÐ÷  
	return RNG_GetRandomNumber();
}

static u32 rng_read( void )
{
	rn_attr.value = rng_getrandomnum()%(rn_attr.max-rn_attr.min+1) + rn_attr.min;
	
	return rn_attr.value; 
}

static s8 rng_ioctrl( u8 cmd, RN_ATTR *prnattr ) 
{
	switch ( cmd )
	{
		case RNCMDSETRANGE :
			rn_attr.min = prnattr->min;
			rn_attr.max = prnattr->max;
			break;
		case RNCMDSETINT :
			break;
		default:
			break;
	}
}



const RN_DIR rng_module = {
	rng_init,
	rng_read,
	rng_ioctrl
};






