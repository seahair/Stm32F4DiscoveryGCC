#include "led.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "delay.h"














/*void LED_Init(void)
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
*/








static void LedInitRed( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  //Only support the GPIOF as OutPut
	GPIO_InitStructure.GPIO_Pin = LEDRED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF, LEDRED );
}

static void SetValueLedRed ( u8 value )
{
	if(value == LedStatus_ON)
		GPIO_ResetBits( GPIOF, LEDRED );
	else if( value == LedStatus_OFF )
		GPIO_SetBits( GPIOF, LEDRED );
}

u8 GetLedStatusRed ( void )
{
	return GPIO_ReadOutputDataBit( GPIOF, LEDRED );
}

void LedOnRed( void )
{
	SetValueLedRed( LedStatus_ON );
}

void LedOffRed( void )
{
	SetValueLedRed( LedStatus_OFF );
}

void LedBlinkRed( u16 time )
{
	LedOnRed( );
	delay_ms( time );
	LedOffRed( );
	delay_ms( time );
}

void LedRollBackRed( void )
{
	(GetLedStatusRed() == LedStatus_ON) ? LedOffRed() : LedOnRed();
}






static void LedInitGreen( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);  //Only support the GPIOF as OutPut
	GPIO_InitStructure.GPIO_Pin = LEDGREEN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOF, LEDGREEN );
}

static void SetValueLedGreen ( u8 value )
{
	if(value == LedStatus_ON)
		GPIO_ResetBits( GPIOF, LEDGREEN );
	else if( value == LedStatus_OFF )
		GPIO_SetBits( GPIOF, LEDGREEN );
}

u8 GetLedStatusGreen ( void )
{
	return GPIO_ReadOutputDataBit( GPIOF, LEDGREEN );
}

void LedOnGreen( void )
{
	SetValueLedGreen( LedStatus_ON );
}

void LedOffGreen( void )
{
	SetValueLedGreen( LedStatus_OFF );
}

void LedBlinkGreen( u16 time )
{
	LedOnGreen( );
	delay_ms( time );
	LedOffGreen( );
	delay_ms( time );
}

void LedRollBackGreen( void )
{
	(GetLedStatusGreen() == LedStatus_ON) ? LedOffGreen() : LedOnGreen();
}




LED LedRed;
LED LedGreen;

void LED_Init( void )
{
	LedRed.pin = LEDRED;
	LedRed.LedInit = LedInitRed;
	LedRed.LedON = LedOnRed;
	LedRed.LedOFF = LedOffRed;
	LedRed.SetValue = SetValueLedRed;
	LedRed.GetLedStatus = GetLedStatusRed;
	LedRed.LedBlink = LedBlinkRed;
	LedRed.LedRollBack = LedRollBackRed;

	LedGreen.pin = LEDGREEN;
	LedGreen.LedInit = LedInitGreen;
	LedGreen.LedON = LedOnGreen;
	LedGreen.LedOFF = LedOffGreen;
	LedGreen.GetLedStatus = GetLedStatusGreen;
	LedGreen.LedBlink = LedBlinkGreen;
	LedGreen.LedRollBack = LedBlinkGreen;
	
}





