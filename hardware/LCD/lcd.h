#ifndef  __LCD_H__
#define	 __LCD_H__

#include "stm32f4xx.h"

#define  CMDADDR		0x6C00007E
#define  DATAADDR		0x6C000080

#define  LCDWRCMD(cmd)			(*(vu16*)(CMDADDR))  = (cmd)
#define  LCDWRDATA(data)		(*(vu16*)(DATAADDR)) = (data)
#define  LCDRDDATA()			(*(vu16*)(DATAADDR)) 

//画笔颜色
#define WHITE			0xFFFF
#define BLACK			0x0000	  
#define BLUE			0x001F  
#define BRED			0XF81F
#define GRED			0XFFE0
#define GBLUE			0X07FF
#define RED				0xF800
#define MAGENTA			0xF81F
#define GREEN			0x07E0
#define CYAN			0x7FFF
#define YELLOW			0xFFE0
#define BROWN			0XBC40 //棕色
#define BRRED			0XFC07 //棕红色
#define GRAY			0X8430 //灰色

//字摸大小类型
#define  ASC2_12		12
#define  ASC2_16		16
#define  ASC2_24		24

//字体显示类型
#define  LCDMODENOBACK	0
#define  LCDMODEADDBACK 1

//扫描方向定义
#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向


typedef enum{
	LCDCMDSLEEPIN  = 0,
	LCDCMDSLEEPOUT,
	LCDCMDSETDIR,
	LCDCMDSETASC2SIZE,
	LCDCMDSETBRUSHCOLOR,
	LCDCMDSETBACKCOLOR,
	LCDCMDSETSHOWMODE
}LCDCMD;

typedef struct _LCD_ATR{
	u16	  width;
	u16   height;
	u16   asc2size;
	u16   showmode;
	u16   brushcolor;
	u16   backcolor;
	u16   cmdwrdata;
	u16   cmdsetx;
	u16   cmdsety;
}lcd_atr_t;	


typedef struct _LCD_DRV{
	u32   (*checkid) ( void );
	void  (*init) ( void );
	void  (*drawpixel) ( u16 x, u16 y, u16 color );
	void  (*fillrect) ( u16 x0, u16 y0, u16 x1, u16 y1, u16 color );
	void  (*drawbitmap) ( u16 x0, u16 y0, u16 width, u16 height, u16* bmp );
	void  (*clear) ( u16 color );
	s8    (*ioctl) ( u32 cmd, u32 param );
	u16   (*getpixel) ( u16 x, u16 y );
	void  (*drawline) ( u16 x0, u16 y0, u16 x1, u16 y1 );
	void  (*showchar) ( u16 x, u16 y, u8 value, u8 size, u8 mode );
	void  (*showstring) ( u16 x, u16 y, u8* p );
}lcd_drv_t;

extern const lcd_drv_t *lcd_drv;

void  LcdInit( void );
void  LcdDrawPixel ( u16 x, u16 y, u16 color );
void  LcdFillRect ( u16 x0, u16 y0, u16 x1, u16 y1, u16 color );
void  LcdDrawBitmap ( u16 x0, u16 y0, u16 width, u16 height, u16* bmp );
void  LcdClear ( u16 color );
s8    LcdIoctl ( u32 cmd, u32 param );
u16   LcdGetPixel (u16 x, u16 y );
void  LcdDrawLine ( u16 x0, u16 y0, u16 x1, u16 y1 );
void  LcdShowChar ( u16 x, u16 y, u8 value, u8 size, u8 mode );
void  LcdShowString ( u16 x, u16 y, u8* p );



#endif
