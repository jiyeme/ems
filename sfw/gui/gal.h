/*
** gal.h: graphic absract layer header file.
**
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#ifndef _SGL_GAL_H
#define _SGL_GAL_H

#include "types.h"
#include "momo.h"

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */






/**
 * \��׼��ɫ
 */
#define COLOR_transparent	0x00000000
#define COLOR_darkred		0x00800000
#define COLOR_darkgreen		0x00008000
#define COLOR_darkyellow	0x00808000
#define COLOR_darkblue		0x00000080
#define COLOR_darkmagenta	0x00800080
#define COLOR_darkcyan		0x00008080
#define COLOR_lightgray		0x00c0c0c0
#define COLOR_darkgray		0x00808080
#define COLOR_red			0x00ff0000
#define COLOR_green			0x0000ff00
#define COLOR_yellow		0x00ffff00
#define COLOR_blue			0x000000f0
#define COLOR_magenta		0x00ff00ff
#define COLOR_cyan			0x0000ffff
#define COLOR_lightwhite	0xF0F0F0
#define COLOR_black			0x00000000


/** ����������ɫ */
#define COLOR_FONT_lpur     0x008028F8  //ǳ��ɫ(purple)�ܳ��� 
#define COLOR_CF            0x00415b74  //����ɫ
#define COLOR_user3         0x0087ceeb  //����ɫ��ҹ��ģʽ��������ɫ
#define COLOR_FONT_night    0x004A6D8C  //QQ�����ҹ������ɫ

/* ����ǰ��ɫ */
#define COLOR_userlabel		0x00949494   
#define COLOR_Combox_range  0x009A9A9A  //combox��Χ���α߿�
#define COLOR_Combox_bg     0x00202020  //combox����

/** ҹ��ģʽ��ɫ */
#define COLOR_bgNight		0x00a1a1a1   //�б�������  
#define COLOR_NFM		    0x0004a6d8
#define COLOR_menuNight		0x00081c10
#define COLOR_Listface	    0x00171a1d  //�б�Ƥ��
#define COLOR_scrbarbg	    0x00e7ece7
#define COLOR_nlabelfont    0x00660008 

/** �����Զ�����ɫ */
#define COLOR_user1			0x00808080
//#define COLOR_user2			0x009090f0
#define COLOR_user4         0x00201c3a


/**
 * \ͼ�γ����
 */
void GAL_Initialize (VOID);

/**
 * \��ֹͼ�γ����
 */
void GAL_Terminate (VOID);

/**
 * \���ÿ�����ص��ֽ�
 */
#define GAL_BytesPerPixel(gc) \
	DIV((gc)->scrInfo.bit + 7 , 8)

/**
 * \���ÿ���ص�λ
 */
#define GAL_BitsPerPixel(gc) \
	((gc)->scrInfo.bit)

/**
 * \��ȡ��Ļ�Ŀ�ȡ�
 */
#define GAL_Width() SCREEN_WIDTH

/**
 * \��ȡ��Ļ�ĸ߶�
 */
#define GAL_Height() SCREEN_HEIGHT

/**
 * \���֧�ֵ���ɫ
 
#define GAL_Colors(gc) \
	(((gc)->scrInfo.bit >= 24)? (1 << 24): (1 << (gc)->scrInfo.bit))
*/
#define GAL_Colors(gc) 16
/**
 * \��ɫ����
 */
#define GAL_FillBox(x, y, w, h, pixel) \
	mrc_drawRect((int16)(x), (int16)(y), (int16)(w), (int16)(h), PIXEL888RED((pixel)), PIXEL888GREEN((pixel)), PIXEL888BLUE((pixel)))

/**
 * \��һ��ˮƽ��
 */
#define GAL_DrawHLine(x, y, w, pixel) \
	mrc_drawLine((int16)(x), (int16)(y), (int16)((x)+(w)-1), (int16)(y), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \��һ����ֱ��
 */
#define GAL_DrawVLine(x, y, h, pixel) \
	mrc_drawLine((int16)(x), (int16)(y), (int16)(x), (int16)((y)+(h)-1), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \����һ������
 */
#define GAL_DrawPixel(x, y, pixel) \
	mrc_drawPointEx((int16)(x), (int16)(y), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \��һ��ֱ��
 */
#define GAL_Line(x1, y1, x2, y2, pixel) \
	mrc_drawLine((int16)(x1), (int16)(y1), (int16)(x2), (int16)(y2), PIXEL888RED(pixel), PIXEL888GREEN(pixel), PIXEL888BLUE(pixel))

/**
 * \��һ�����ο�
 */
void GAL_Rectangle(int x, int y, int w, int h, Uint32 pixel);

/**
 * \����һ�������ض���ȵľ��ο�
 */
void GAL_Rectangle2(int x, int y, int w, int h, int line, Uint32 pixel);

/**
 * \ˢ����Ļ
 */
#define GAL_Flush() \
	mrc_refreshScreen(0, 0, (uint16)SCREEN_WIDTH, (uint16)SCREEN_HEIGHT)

/**
 * \ˢ����Ļ������
 */
#define GAL_FlushRegion(x,  y,  w, h) \
	mrc_refreshScreen((int16)(x), (int16)(y), (uint16)(w), (uint16)(h))



#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* _SGL_GAL_H */

