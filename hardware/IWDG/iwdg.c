#include "iwdg.h"
#include "stm32f4xx_iwdg.h"




void IwdgStart1S( void )
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); //取消寄存器写保护
	IWDG_SetPrescaler( PRER1S ); //设置 IWDG 分频系数
	IWDG_SetReload( RLR1S ); //设置 IWDG 装载值
	IWDG_ReloadCounter(); //reload
	IWDG_Enable(); //使能看门狗
}

void IwdgFeed( void )
{
	IWDG_ReloadCounter(); //reload
}


