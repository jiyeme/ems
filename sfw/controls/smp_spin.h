#ifndef _SMP_SPIN_H
#define _SMP_SPIN_H

#include "window.h"

/**
  \����

  ��Ҫʹ�ü򵥵�������ʾ��
 - ������������
 - ������ת��Χ
 - ��ӵ�������
 - ��Ӧ��Ϣ������֪ͨ

  
  \code
	//����һ����ת��
	hControl = SGL_CreateWindow(SMP_Spin_WndProc, ...);
	SMP_Spin_SetRange(hControl,  ...);
	SGL_AddChildWindow(hWnd, hControl);

	//��Ӧ��Ϣ֪ͨ��ת
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the spin id")
		{
			switch(code)
			{
			case SMP_SPINN_VALUECHANGED:
				Sint32 value = (Sint32)lParam; //����ֵ

				//����֪ͨ�¼�
				break;
			}
		}	
  \endcode
  



������ʽ


 * \��ʾ�������
 */
#define SMP_SPINS_LEFTARROW			0x0001L

/**
 * \��ʾ�����Ҽ�
 */
#define SMP_SPINS_RIGHTARROW		0x0002L

/**  
����֪ͨ��Ϣ

 * \��תʱ���ʹ���ֵ�ı���Ϣ
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the spin id" && code == SMP_SPINN_VALUECHANGED)
 *		{
			Sint32 value = (Sint32)lParam; //����ֵ

 * 			//����������֪ͨ��Ϣ
 *		}
 * \endcode
 *
 * \����hControl	��ת�����ʹ���Ϣ
 */
#define SMP_SPINN_VALUECHANGED		0x0001

/** 
���ڳ�Ա����

 * ���ô��ڷ�Χ�ڵ�����
 *
 * \����hWnd	�������ھ��
 * \����min		��Сֵ
 * \����max		���ֵ
 * \����step	����
 */
VOID SMP_Spin_SetRange(HWND hWnd, Sint32 min, Sint32 max, Sint32 step);

/**
 * \���õ�ǰֵ
 *
 * \����hWnd		�������ھ��
 * \����param		��ǰֵ
 * \����notify		����֪ͨ
 * \����update		���´���
 */
VOID SMP_Spin_SetValue(HWND hWnd, Sint32 value, BOOL notify, BOOL update);

/**
 * \��ȡ�������ڵĵ�ǰֵ
 *
 * ����hWnd	�������ھ��
 * \���ص�ǰֵ
 */
Sint32 SMP_Spin_GetValue(HWND hWnd);

/** 
 * \�������ڳ���
 *
 * \����hWnd		���ھ��
 * \����Msg			������Ϣ
 * \����wParam		��һ������
 * \����wParam		��һ������
 * \������Ϣ�Ĵ�����
 */
LRESULT SMP_Spin_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	

#endif
