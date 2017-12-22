#ifndef  __FLASH_H__
#define  __FLASH_H__

#include "stm32f4xx.h"

#define  FLASHMAXADDR		0X080FFFFF
#define STM32_FLASH_BASE	0x08000000

#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000)	
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000)
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000)
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000)
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000)
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000)
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000)
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000)
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000)
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000)
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000)
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000)

#if 0
typedef struct _FLASH_ATTR{
	u32 waddr;
	u32 *wbuf;
	u32 wlen;
	u32 raddr;
	u32 *rbuf;
	u32 rlen;
}FLASH_ATTR;

typedef struct _FLASH_DIR{
	void (*init) ( void );
	s8 (*read) ( u32 addr, u32 *buf, u32 len );
	s8 (*write) ( u32 addr, u32 *buf, u32 len );
}FLASH_DIR;

void FlashInit( void );
s8 FlashRead( u32 addr, u32 *buf, u32 len );
s8 FlashWrite( u32 addr, u32 *buf, u32 len );
#endif

typedef struct MYFLASH MYFLASH;

typedef s8 (*FLASHREAD) ( MYFLASH * const me, u32 addr, u32 *buf, u32 len );
typedef s8 (*FLASHWRIET) ( MYFLASH * const me, u32 addr, u32 *buf, u32 len );
struct MYFLASH{
	u8 sta;
	FLASHREAD read;
	FLASHWRIET write;
};

MYFLASH *FlashCreat( void );
void FlashDestroy( MYFLASH * const me );

#endif
