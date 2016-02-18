#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f10x.h"
#include "integer.h"

#define LCD_WIDTH		128
#define LCD_HIGHT		64									
#define LCD_RAM_SIZE	(LCD_WIDTH*LCD_HIGHT/8)

//4*5点阵字符定义
#define LCD_MN_DOT		10
#define LCD_MN_MINUS	11
#define LCD_MN_MW1		12
#define LCD_MN_MW2		13
#define LCD_MN_MW3		14
#define LCD_MN_SP		15
#define LCD_MN_PLUS		16
#define LCD_MN_SPA		17
#define LCD_MN_COL		18
#define LCD_MN_A		23
#define LCD_MN_CHAR(X)	(X-'A'+LCD_MN_A)

//位图文件头信息结构定义
//其中不包含文件类型信息（由于结构体的内存结构决定，要是加了的话将不能正确读取文件信息）
typedef struct 
{
    DWORD bfSize; 														//文件大小											4																		4
    WORD bfReserved1; 													//保留字，不考虑										2
    WORD bfReserved2; 													//保留字，同上										2					 															2
    DWORD bfOffBits; 													//实际位图数据的偏移字节数，即前三个部分长度之和 		4
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;

typedef struct 
{
    DWORD biSize; 														//指定此结构体的长度，为40 							4
    LONG biWidth; 														//位图宽												4									 												4
    LONG biHeight; 														//位图高												4									 												4
    WORD biPlanes; 														//平面数，为1										2						 													2
    WORD biBitCount; 													//采用颜色位数，可以是1，2，4，8，16，24新的可以是32	2
    DWORD biCompression; 												//压缩方式，可以是0，1，2，其中0表示不压缩				4
    DWORD biSizeImage; 													//实际位图数据占用的字节数							4
    LONG biXPelsPerMeter; 												//X方向分辨率										4																				4
    LONG biYPelsPerMeter; 												//Y方向分辨率										4											4
    DWORD biClrUsed; 													//使用的颜色数，如果为0，则表示默认值(2^颜色位数)		4
    DWORD biClrImportant; 												//重要颜色数，如果为0，则表示所有颜色都是重要的		4
	DWORD blank[2];														//空白												8
} BITMAPINFOHEADER,tagBITMAPINFOHEADER;

void LCD_BL_Cof(void);
void LCD_SetBL(u8 brightness);
void LCD_SetContrast(u8 contrast);
void LCD_Init(void);
void LCD_Clr_All(void);
void LCD_Disp_All(void);
void LCD_Refresh_Frame(void);
void LCD_Disp_Dot(u16 row,u16 column,u8 color);

void Disp_Graphic_6x8(u16 row,u16 column,u8 *dp,u8 underline,u8 reverse);
void Disp_String_6x8(u16 row,u16 column,u8 *text,u8 underline,u8 reverse);
void Disp_Graphic_12x12(u16 row,u16 column,u8 *dp,u8 underline,u8 reverse);
void Disp_Graphic_6x12(u16 row,u16 column,u8 *dp,u8 underline,u8 reverse);
void Disp_GB2312_String(u16 row,u16 column,u8 *text,u8 underline,u8 reverse);

void LCD_Disp_BMP(u16 row,u16 column,char *pic_name);

void LCD_Draw_Line(s16 x1,s16 y1,s16 x2,s16 y2,u8 color);
void LCD_Draw_Rect(s16 x1,s16 y1,s16 x2,s16 y2,u8 color);
void LCD_Fill_Rect(s16 x1,s16 y1,s16 x2,s16 y2,u8 color);

void LcdDrawInt(u16 x,u16 y,s16 value,u8 n,u8 dot,u8 plus,u8 ar,u8 underline,u8 reverse);

void LcdDrawMaskY(u16 x,u16 y,u8 mask,u8 n,u8 reverse);
void LcdDrawMaskX(u16 x,u16 y,u8 mask,u8 n,u8 reverse);
void LcdDrawMiniNum(u16 x,u16 y,u8 num,u8 reverse);
void LcdDrawMiniInt(u16 x,u16 y,s16 value,u8 n,u8 dot,u8 plus,u8 ar,u8 reverse);
void LcdDrawMiniEng(u16 x,u16 y,char *eng,u8 reverse);

void LcdDrawIcon(u16 x,u16 y,u16 w,u16 h,const u8 *icon);
void LcdDrawUsbLogo(u16 x,u16 y);

void LcdDrawTxRun(u16 x,u16 y);
void LcdDrawThunder(u16 x,u16 y);
void LcdDrawClock(u16 x,u16 y);

void LcdDrawArrow(u16 x,u16 y,u8 focus);
void LcdDraw_ValueArrow(u16 x,u16 y,u8 state);
void LcdDraw_EditArrow(u16 x,u16 y);
void LcdDraw_RevBarArrow(u16 x,u16 y);

void LCDHandler(void);
extern u32 LightStartTmr;//背光时间计时器

void Lcd_CloseFile(void);

#endif
