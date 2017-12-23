#include "malloc.h"


u8 extimembase[EXTIMEMSIZE] __attribute__((section(".glx"))) __attribute__((align(32)));	//外部SRAM内存池
u16 extimemmangebase[EXTIMEMALLOCTABLE];  //外部SRAM内存池MAP

static void memset( void *addr, u32 len, u8 data )
{
	u8 *paddr = addr;

	while(len--)
		*(paddr++) = data;
}


static void *mymalloc( MYMALLOC *me, u32 size )
{
}

static void *myrealloc( MYMALLOC *me, u32 size )
{
}

static void myfree( MYMALLOC *me, void *p )
{
}

static u8 mypreuse( MYMALLOC *me )
{
	u32 i, used=0;

	for( i=0; i<me->blocknum; i++ )
	{
		if( me->memmangebase[i] )
			used++;	
	}

	return (used*100)/me->blocknum;
}

static void MymallocInit( MYMALLOC *me, MEMMALLOC malloc, MEMREALLOC realloc, MEMFREE free, MEMPREUSE preuse )
{
	me->sta = MEMUNINIT;
	me->memsize = EXTIMEMSIZE;
	me->blocksize = EXTIMEMBLOCKSIZE;
	me->blocknum = me->memsize/me->blocksize;
	me->membase = extimembase;
	me->memmangebase = extimemmangebase;
	me->malloc = malloc;
	me->realloc = realloc;
	me->free = free;
	me->preuse = preuse;

	memset( me->membase, me->memsize, NULL );
	memset( me->memmangebase, me->blocknum*2, NULL );
	me->sta = MEMINIT;
}


MYMALLOC *MallocCreat( void )
{
	static MYMALLOC extimemalloc;

	MymallocInit( &extimemalloc, mymalloc, myrealloc, myfree, mypreuse );
}

void MallocDestroy( MYMALLOC *me )
{

}
