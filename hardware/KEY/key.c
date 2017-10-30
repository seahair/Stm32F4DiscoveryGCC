#include "key.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"


void KeyPinInit( void )
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = PINKEY0|PINKEY1|PINKEY2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PINKEYWK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
}


s8 GetKeyStatus( u8 pin )
{
	switch (pin)
	{
		case KEY0:
			return GPIO_ReadOutputDataBit( GPIOE, PINKEY0 );
			break;
		case KEY1:
			return GPIO_ReadOutputDataBit( GPIOE, PINKEY1 );
			break;
		case KEY2:
			return GPIO_ReadOutputDataBit( GPIOE, PINKEY2 );
			break;
		case KEYWK:
			return GPIO_ReadOutputDataBit( GPIOA, PINKEYWK );
			break;
		default:
			return -1;
	}
}

u8 WhichKeyPress( void )
{
	u8 i;
	while(1)
	{
		if( KEY0PRESS == GetKeyStatus( KEY0 ) )
		{
			delay_ms(100);
			if( KEY0PRESS == GetKeyStatus( KEY0 ) )
				return KEY0;
		}

		if( KEY1PRESS == GetKeyStatus( KEY1 ) )
		{
			delay_ms(100);
			if( KEY1PRESS == GetKeyStatus( KEY1 ) )
				return KEY1;
		}

		if( KEY2PRESS == GetKeyStatus( KEY2 ) )
		{
			delay_ms(100);
			if( KEY0PRESS == GetKeyStatus( KEY2 ) )
				return KEY2;
		}

		if( KEYWKPRESS == GetKeyStatus( KEYWK ) )
		{
			delay_ms(100);
			if( KEYWKPRESS == GetKeyStatus( KEYWK ) )
				return KEYWK;
		}

	}
}


void KeyTest( KEYTEST pFun )
{
	KEYTEST  KeyTestHandle = pFun;
	KeyTestHandle( WhichKeyPress );
}






