#include "malloc.h"


u8 extimembase[EXTIMEMSIZE] __attribute__((section(".glx"))); __attribute__((aligned(32)));	//外部SRAM内存池
u16 extimemmangebase[EXTIMEMALLOCTABLE];  //外部SRAM内存池MAP

u8 inmembase[INMEMSIZE]; 
u16 inmemmangebase[INMEMALLOCTABLE];  

u8 ccmmembase[CCMMEMSIZE] __attribute__((section(".ccmram"))); __attribute__((aligned(32)));	//外部SRAM内存池
u16 ccmmemmangebase[CCMMEMALLOCTABLE];  //外部SRAM内存池MAP


MYMALLOC extimemalloc;
MYMALLOC inmemalloc;
MYMALLOC ccmmemalloc;

static void memset( void *addr, u32 len, u8 data )
{
	u8 *paddr = addr;

	while(len--)
	{
		*(paddr++) = data;
		//paddr += 2;
	}
}


static void *mymalloc( MYMALLOC *me, u32 size )
{
	u16 num;
	u32 i,j;

	if( size == 0 )
		return NULL;

	if( size<me->memsize && (me->sta==MEMINIT) )
	{
		num = (size/(me->blocksize)) + (((size%(me->blocksize))>0)?1:0);		
		for( i=0; i<me->blocknum; i++ )
		{
			if( me->memmangebase[i]!=0 )
				continue;
			else
			{
				if( i+num < me->blocknum )
				{
					for( j=0; j<num; j++ )
						me->memmangebase[i+j] = num;
					break;
				}
				else
					return NULL;	
			}
		}	
	}
	else
		return NULL;

	return me->membase+i*me->blocksize;
}

static void *myrealloc( MYMALLOC *me, u32 size )
{
	
}

static void myfree( MYMALLOC *me, void *p )
{
	if( p!=NULL && (p>me->membase) && (p<(me->membase+me->memsize)) )
	{
		u32 offset = (u8*)p - me->membase;
		u16 index = offset/(me->blocksize);
		u16 bnum = me->memmangebase[index];
		for( u16 i=0; i<index; i++ )
		{
			me->memmangebase[index+i] = NULL;	
		}
	}
}

static u8 mypreuse( MYMALLOC *me )
{
	u32 i, used=0;

	for( i=0; i<me->blocknum; i++ )
	{
		if( me->memmangebase[i] )
			used++;	
	}

	return (used*100)/(me->blocknum);
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
	
	return &extimemalloc;
}

void MallocDestroy( MYMALLOC *me )
{

}

void ExtiMallocInit( void )
{
	extimemalloc.sta = MEMUNINIT;
	extimemalloc.memsize = EXTIMEMSIZE;
	extimemalloc.blocksize = EXTIMEMBLOCKSIZE;
	extimemalloc.blocknum = extimemalloc.memsize/extimemalloc.blocksize;
	extimemalloc.membase = extimembase;
	extimemalloc.memmangebase = extimemmangebase;
	extimemalloc.malloc = mymalloc;
	extimemalloc.realloc = myrealloc;
	extimemalloc.free = myfree;
	extimemalloc.preuse = mypreuse;

	memset( extimemalloc.membase, extimemalloc.memsize, NULL );
	memset( extimemalloc.memmangebase, extimemalloc.blocknum*2, NULL );
	extimemalloc.sta = MEMINIT;
}

void InMallocInit( void )
{
	inmemalloc.sta = MEMUNINIT;
	inmemalloc.memsize = INMEMSIZE;
	inmemalloc.blocksize = INMEMBLOCKSIZE;
	inmemalloc.blocknum = inmemalloc.memsize/inmemalloc.blocksize;
	inmemalloc.membase = inmembase;
	inmemalloc.memmangebase = inmemmangebase;
	inmemalloc.malloc = mymalloc;
	inmemalloc.realloc = myrealloc;
	inmemalloc.free = myfree;
	inmemalloc.preuse = mypreuse;

	memset( inmemalloc.membase, inmemalloc.memsize, NULL );
	memset( inmemalloc.memmangebase, inmemalloc.blocknum*2, NULL );
	inmemalloc.sta = MEMINIT;
}

void CcmMallocInit( void )
{
	ccmmemalloc.sta = MEMUNINIT;
	ccmmemalloc.memsize = INMEMSIZE;
	ccmmemalloc.blocksize = INMEMBLOCKSIZE;
	ccmmemalloc.blocknum = ccmmemalloc.memsize/ccmmemalloc.blocksize;
	ccmmemalloc.membase = ccmmembase;
	ccmmemalloc.memmangebase = ccmmemmangebase;
	ccmmemalloc.malloc = mymalloc;
	ccmmemalloc.realloc = myrealloc;
	ccmmemalloc.free = myfree;
	ccmmemalloc.preuse = mypreuse;
	
	memset( ccmmemalloc.membase, ccmmemalloc.memsize, NULL );
	memset( ccmmemalloc.memmangebase, ccmmemalloc.blocknum*2, NULL );
	ccmmemalloc.sta = MEMINIT;
}





