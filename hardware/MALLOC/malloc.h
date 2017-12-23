#ifndef  __MALLOC_H__
#define  __MALLOC_H__

#include "stm32f4xx.h"

#define  MEMUNINIT					0
#define  MEMINIT					1
#define  NULL						0

#define  EXTIMEMBLOCKSIZE			32
#define  EXTIMEMSIZE				(960*1024)
#define  EXTIMEMALLOCTABLE			EXTIMEMSIZE/EXTIMEMBLOCKSIZE

 
typedef struct MYMALLOC MYMALLOC;

typedef void *(*MEMMALLOC) ( MYMALLOC *me, u32 size );
typedef void *(*MEMREALLOC) ( MYMALLOC *me, u32 size );
typedef void (*MEMFREE) ( MYMALLOC *me, void *p );
typedef u8 (*MEMPREUSE) ( MYMALLOC *me );


struct MYMALLOC {
	u8 sta;
	u32 memsize;
	u16 blocksize;
	u32 blocknum;
	u8 *membase;
	u16 *memmangebase;
	MEMMALLOC malloc;
	MEMREALLOC realloc;
	MEMFREE free;
	MEMPREUSE preuse;
};

MYMALLOC *MallocCreat( void );
void MallocDestroy( MYMALLOC *me );

#endif
