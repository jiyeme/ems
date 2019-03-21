#ifndef _SMP_TITLEBAR_H
#define _SMP_TITLEBAR_H

#include "window.h"

//������Ψһ��ʽ
#define SMP_TITLEBAR_STATIC		0x0002L

//��ʾ����ʱ��
VOID ShowLocalTime(uint32 color);

//���ñ������ұߵ��ַ���(ʱ��)
VOID SMP_Titlebar_SetContentR(HWND hWnd, PCWSTR content, BOOL redraw);

//���ñ���������
VOID SMP_Titlebar_SetContent(HWND hWnd, DWORD bmpID, PCWSTR content, BOOL redraw);

//�����������¼�
LRESULT SMP_Titlebar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	
#endif
