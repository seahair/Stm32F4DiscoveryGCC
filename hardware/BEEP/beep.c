#include "beep.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "delay.h"


static u8 SONG_HZ[]={212,212,190,212,159,169,212,212,190,212,142,159,212,212,106,126,159,169,190,119,119,126,159,142,159,0};

static u8 SONG_TIME[]={9,3,12,12,12,24,9,3,12,12,12,24,9,3,12,12,12,12,12,9,3,12,12,12,24,0};





void BeepInit( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BEEPPIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOF, BEEPPIN);
}

void BeepOn( void )
{
	GPIO_SetBits( GPIOF, BEEPPIN );	
}

void BeepOff( void )
{
	GPIO_ResetBits( GPIOF, BEEPPIN );
}

void BeepOnHzTime( u16 hz, u32 time )
{
	u32 num = time*hz/1000 ;
	while( num--  )
	{
		BeepOn( );
		delay_us( (500000/hz)*1 );
		BeepOff( );
		delay_us( (500000/hz)*1 );
	}
}

void PlayMusic( void )
{
	u16 i = 0;
	while(SONG_HZ[i]!=0 || SONG_TIME[i]!=0)
	{
		BeepOnHzTime(SONG_HZ[i], SONG_TIME[i]*40);
		i++;
	}
}

BEEP Beep;



