#ifndef _SMP_H
#define _SMP_H

#include "types.h"

//�û��������ɫ
#define COLOR_focus				0x00ffae00//0x00fe9900
#define COLOR_border			0x000a3a73
#define COLOR_controlbg			0x00e7f1f3
#define COLOR_controlhili		0x0075a4c7
#define COLOR_controlborder		0x00cfe3fe
#define COLOR_wndheader			0x00074c87


extern uint8 SMP_ITEM_HEIGHT;
extern uint16 SCREEN_WIDTH;	
extern uint16 SCREEN_HEIGHT;

#define SMP_CHECKBOX_SIZE  16
#define SMP_ITEM_SPACE      2
#define SMP_ITEM_MARGIN     5

#define SMP_ITEM_LENGTH				(SCREEN_WIDTH - 2*SMP_ITEM_MARGIN)
#define SMP_ITEM_SPACE_HEIGHT		(SMP_ITEM_HEIGHT + SMP_ITEM_SPACE)
#define SMP_CONTENT_VIEW_WIDTH		SCREEN_WIDTH
#define SMP_CONTENT_VIEW_HEIGHT		(SCREEN_HEIGHT - 2*SMP_ITEM_HEIGHT)
#define SMP_SCRBAR_WIDTH			4
#define SMP_MENU_BOTTOM_HEIGHT		SMP_ITEM_HEIGHT
#define SMP_LIST_MARGIN		        3	

/**
 * \��/�ҿռ�
 */
#define SMP_ITEM_CONTENT_MARGIN		4 //����

/**
 * \���Ƹ�ѡ���ܡ�
 * \����x		���λ��
 * \����Y		����λ��
 * \����check	�Ƿ�ѡ�и�ѡ��
 */
VOID SMP_DrawCheckBox(int x, int y, BOOL checked);

/**
 * \����Ϣ����
 */
VOID SMP_DrawMsgBoxFrame(int x, int y, int w, int h);

/**
 * \����һ�����㴰�ڵı��⡣
 *
 * \����x			Ϊ��MsgBox���λ��
 * \����y			����λ�õ���MsgBox
 * \����w			�����MsgBox
 * \����h			�߶���MsgBox
 * \����bgcolor		������ɫ
 * \����fgcolor		ǰ��ɫ
 * \����bmpID		λͼ��ID
 * \����str			����ͷ
 */
VOID SMP_DrawWndHeader(int x, int y, int w, int h, Uint32 bgcolor, Uint32 fgcolor, DWORD bmpID, PCWSTR str);


#endif


