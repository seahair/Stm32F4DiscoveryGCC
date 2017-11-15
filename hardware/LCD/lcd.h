#ifndef __LCD_H
#define __LCD_H		
#include "sys.h"	 
#include "stdlib.h" 

typedef struct  
{										    
	u16 width;			//LCD 宽度
	u16 height;			//LCD 高度
	u16 id;				//LCD ID
	u8 dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。	
	u16	wramcmd;		//开始写gram指令
	u16  setxcmd;		//设置x坐标指令
	u16  setycmd;		//设置y坐标指令 
}_lcd_dev; 

extern _lcd_dev lcddev;	
//extern u16 Guolinxin;
extern u32 Guolinxin;


typedef struct
{
	vu16 LCD_REG;
	vu16 LCD_RAM;
} LCD_TypeDef;
#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))
#define LCD             ((LCD_TypeDef *) LCD_BASE)
	 
#define CMD_ADDR    0x6C00007E
#define DATA_ADDR   0x6C000080      

#define  LCD_WR_REG(cmd)   (*(volatile uint16_t *)(CMD_ADDR)) = (cmd)
#define  LCD_WR_DATA(val)   (*(volatile uint16_t *)(DATA_ADDR)) = (val)
#define  LCD_RD_DATA()   (*(volatile uint16_t *)(DATA_ADDR))


#define L2R_U2D  0 //从左到右,从上到下
#define L2R_D2U  1 //从左到右,从下到上
#define R2L_U2D  2 //从右到左,从上到下
#define R2L_D2U  3 //从右到左,从下到上

#define U2D_L2R  4 //从上到下,从左到右
#define U2D_R2L  5 //从上到下,从右到左
#define D2U_L2R  6 //从下到上,从左到右
#define D2U_R2L  7 //从下到上,从右到左	 

#define DFT_SCAN_DIR  L2R_U2D  //默认的扫描方向

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色

#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
 
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
	    															  
void LcdTest( u8 num );
void LcdTest2( u32* val , _lcd_dev* dev );


void LCD_Init(void);												
void LCD_DisplayOn(void);											
void LCD_DisplayOff(void);											
void LCD_Clear(u16 Color);	 										
void LCD_SetCursor(u16 Xpos, u16 Ypos);								
void LCD_DrawPoint(u16 x,u16 y);									
void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color);						
u16  LCD_ReadPoint(u16 x,u16 y); 									
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r);						 	
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);					
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);		   		
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color);		   		
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color);		
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);				
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size);  				
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);		
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,u8 *p);

void LCD_WriteReg(u16 LCD_Reg, u16 LCD_RegValue);
u16 LCD_ReadReg(u16 LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(u16 RGB_Code);
void LCD_SSD_BackLightSet(u8 pwm);							
void LCD_Scan_Dir(u8 dir);								
void LCD_Display_Dir(u8 dir);						
void LCD_Set_Window(u16 sx,u16 sy,u16 width,u16 height);

#define SSD_HOR_RESOLUTION		800
#define SSD_VER_RESOLUTION		480
#define SSD_HOR_PULSE_WIDTH		1
#define SSD_HOR_BACK_PORCH		46
#define SSD_HOR_FRONT_PORCH		210

#define SSD_VER_PULSE_WIDTH		1
#define SSD_VER_BACK_PORCH		23
#define SSD_VER_FRONT_PORCH		22
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)

#endif  
	 
	 



