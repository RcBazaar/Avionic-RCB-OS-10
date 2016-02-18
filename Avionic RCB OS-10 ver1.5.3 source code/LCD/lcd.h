#ifndef _LCD_H_
#define _LCD_H_

#include "stm32f10x.h"
#include "integer.h"

#define LCD_WIDTH		128
#define LCD_HIGHT		64									
#define LCD_RAM_SIZE	(LCD_WIDTH*LCD_HIGHT/8)

//4*5�����ַ�����
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

//λͼ�ļ�ͷ��Ϣ�ṹ����
//���в������ļ�������Ϣ�����ڽṹ����ڴ�ṹ������Ҫ�Ǽ��˵Ļ���������ȷ��ȡ�ļ���Ϣ��
typedef struct 
{
    DWORD bfSize; 														//�ļ���С											4																		4
    WORD bfReserved1; 													//�����֣�������										2
    WORD bfReserved2; 													//�����֣�ͬ��										2					 															2
    DWORD bfOffBits; 													//ʵ��λͼ���ݵ�ƫ���ֽ�������ǰ�������ֳ���֮�� 		4
} BITMAPFILEHEADER,tagBITMAPFILEHEADER;

typedef struct 
{
    DWORD biSize; 														//ָ���˽ṹ��ĳ��ȣ�Ϊ40 							4
    LONG biWidth; 														//λͼ��												4									 												4
    LONG biHeight; 														//λͼ��												4									 												4
    WORD biPlanes; 														//ƽ������Ϊ1										2						 													2
    WORD biBitCount; 													//������ɫλ����������1��2��4��8��16��24�µĿ�����32	2
    DWORD biCompression; 												//ѹ����ʽ��������0��1��2������0��ʾ��ѹ��				4
    DWORD biSizeImage; 													//ʵ��λͼ����ռ�õ��ֽ���							4
    LONG biXPelsPerMeter; 												//X����ֱ���										4																				4
    LONG biYPelsPerMeter; 												//Y����ֱ���										4											4
    DWORD biClrUsed; 													//ʹ�õ���ɫ�������Ϊ0�����ʾĬ��ֵ(2^��ɫλ��)		4
    DWORD biClrImportant; 												//��Ҫ��ɫ�������Ϊ0�����ʾ������ɫ������Ҫ��		4
	DWORD blank[2];														//�հ�												8
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
extern u32 LightStartTmr;//����ʱ���ʱ��

void Lcd_CloseFile(void);

#endif
