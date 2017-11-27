#include "rng.h"
#include "randomnum.h"


static void rng_init( void )
{

}

static u32 rng_read( void )
{

}

static s8 rng_ioctrl( u8 cmd, RN_ATTR *prnattr ) 
{

}



const RN_DIR rng_module = {
	rng_init,
	rng_read,
	rng_ioctrl
};






