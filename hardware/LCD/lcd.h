#ifndef  __LCD_H__
#define  __LCD_H__

#include "stm32f4xx.h"



typedef struct
{
	u16 LCD_CMD;	// cmd
	u16 LCD_DATA;	// data
} LCD_TypeDef;		//LCD 操作结构体


#define LCD_BASE ((u32)(0x6C000000 | 0x0000007E))
#define LCD ((LCD_TypeDef *) LCD_BASE)


typedef struct 
{ 
	u16 width; //LCD 宽度
	u16 height; //LCD 高度
	u16 id; //LCD ID
	u8 dir; //横屏还是竖屏控制：0，竖屏；1，横屏。
	u16 wramcmd; //开始写 gram 指令
	u16 setxcmd; //设置 x 坐标指令
	u16 setycmd; //设置 y 坐标指令
}_lcd_dev;		//LCD 重要参数集

extern _lcd_dev lcddev; //管理 LCD 重要参数





//void LcdPinInit( void );
//void LcdFsmcInit( void );
//void LcdFsmcEnable( void );
//void LcdFsmcDisable( void );
void LcdInit( void );

void LcdWriteCmd( u16 value );
void LcdWriteData( u16 value );
u16  LcdReadCmd( void );
u16  LcdReadData( void );



#endif
