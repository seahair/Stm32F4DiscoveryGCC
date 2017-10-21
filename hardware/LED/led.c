#include "led.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "delay.h"

void LED_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//Ê¹ÄÜGPIOFÊ±ÖÓ
	GPIO_InitStructure.GPIO_Pin = LedRed | LedGreen;//LED0ºÍLED1¶Ô
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍšÊä³öÄ£Êœ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
	GPIO_Init(GPIOF, &GPIO_InitStructure);//³õÊŒ»¯GPIO
	
	GPIO_SetBits(GPIOF, LedRed | LedGreen);//GPIOF9,F10ÉèÖÃžß£¬µÆÃð

}


void LedInit( u16 pin )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  //Only support the GPIOF as OutPut
	GPIO_InitStructure.GPIO_Pin = pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF, pin);
}

void LedON( u16 pin )
{
	GPIO_ResetBits(GPIOF, pin);
}

void LedOFF( u16 pin )
{
	GPIO_SetBits(GPIOF, pin);
}

void LedBlink( u16 pin )
{
	LedON( pin );
	delay_ms( 1000 );
	LedOFF( pin );
	delay_ms( 1000 );
}

u8 GetLedStatus( u16 pin )
{
	return GPIO_ReadOutputDataBit(GPIOF, pin);
}

void LedRollBack( u16 pin )
{
	(GetLedStatus(pin) == LedStatus_ON)? LedOFF(pin) : LedON(pin) ;
}


