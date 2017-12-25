#include "malloc.h"


//u8 extimembase[2];
u8 extimembase[EXTIMEMSIZE] __attribute__((section(".glx"))); __attribute__((aligned(32)));	//外部SRAM内存池
u16 extimemmangebase[EXTIMEMALLOCTABLE];  //外部SRAM内存池MAP

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