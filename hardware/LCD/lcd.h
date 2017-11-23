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



typedef enum{
	LCDCMDSLEEPIN  = 0,
	LCDCMDSLEEPOUT,
	LCDCMDSETDIRH,
	LCDCMDSETDIRV
}LCDCMD;

typedef struct _LCD_ATR{
	u16	  width;
	u16   height;
	u16   id;
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
	u16   (*getpixel) (u16 x, u16 y );
}lcd_drv_t;

extern const lcd_drv_t *lcd_drv;

void  LcdInit( void );
void  LcdDrawPixel ( u16 x, u16 y, u16 color );
void  LcdFillRect ( u16 x0, u16 y0, u16 x1, u16 y1, u16 color );
void  LcdDrawBitmap ( u16 x0, u16 y0, u16 width, u16 height, u16* bmp );
void  LcdClear ( u16 color );
s8    LcdIoctl ( u32 cmd, u32 param );
u16   LcdGetPixel (u16 x, u16 y );



#endif
