#include "stm32f4xx.h"
#include "flash.h"
#include "stm32f4xx_flash.h"



static u32 STMFLASH_ReadWord(u32 faddr)
{
	return *(vu32*)faddr;
}

s8 FlashRead( MYFLASH* const me, u32 addr, u32 *buf, u32 len )
{
	if( addr%4 ) 
		return -2;

	if( addr<FLASHMAXADDR && (addr+len)<FLASHMAXADDR )
	{
		u8 i=0;
	
		for( i=0; i<len; i++ )
		{
			*(buf+i) = STMFLASH_ReadWord( addr+i*4 );
		}

		return 0;
	}
	else 
		return -1;
}


static uint16_t STMFLASH_GetFlashSector(u32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 

	return FLASH_Sector_11;
}

s8 FlashWrite( MYFLASH* const me, u32 addr, u32 *buf, u32 len )
{
	u32 i;

	if( addr<STM32_FLASH_BASE || addr>FLASHMAXADDR || (addr+len)>FLASHMAXADDR )
		return -1;
	if( addr%4 ) 
		return -2;
	
	FLASH_Unlock(); //解锁
	FLASH_DataCacheCmd(DISABLE);//FLASH 擦除期间,必须禁止数据缓存
	
	if( FLASH_EraseSector(STMFLASH_GetFlashSector(addr),VoltageRange_3) != FLASH_COMPLETE )
		return -3;

	for( i=0; i<len; i++ )
	{
		if(FLASH_ProgramWord( addr+i*4, *(buf+i) )!=FLASH_COMPLETE)//写入数据
			return -4;
	}

	FLASH_DataCacheCmd(ENABLE); //FLASH 擦除结束,开启数据缓存
	FLASH_Lock();//上锁

}


static void FlashInit( MYFLASH* const me, FLASHREAD read, FLASHWRITE write )
{
	me->sta = 0;
	me->read = read;
	me->write = write;
}

static void FlashCleanup( MYFLASH* const me )
{

}

MYFLASH *FlashCreat( void )
{
	static MYFLASH sflash;

	FlashInit( &sflash, FlashRead, FlashWrite );	

	return &sflash;
}

void FlashDestroy( MYFLASH * const me )
{

}


