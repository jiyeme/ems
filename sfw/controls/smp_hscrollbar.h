#ifndef _SMP_HSCROLLBAR_H
#define _SMP_HSCROLLBAR_H

#include "window.h"

/* ������

��Ҫ�û�������
   - ��������������
   - ���û��������ڵ���Ϣ
   - ��ӵ�������
   - ��Ӧ��Ϣ��֪ͨ

  \code
  	//����������
  	hHScrbar = SGL_CreateWindow(SMP_HScrollbar_WndProc, ...);
  	SMP_HScrollbar_SetInfo(hHScrbar);
  	SGL_AddChildWindow(hWnd, hHScrbar);

  	//��Ӧ��Ϣ��֪ͨ
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);
		if(id == "the Hscrollbar id" && code == SMP_HSCRBARN_VALUECHANGED)
		{
			int index = (int)lParam; //��ǰ������ֵ
		}	
  \endcode


 *
 * \����index   �������ĵ�ǰֵ
 */
#define SMP_HSCRBARN_VALUECHANGED		0x0001


//�������ĵ�ǰֵ���õ������
#define SMP_HSCRBARN_LAST  2

//�̶��߶�
#define SMP_HSCRBARN_HEIGHT 25

/** 
 * \���û���������Ϣ
 *
 * \����hWnd        ���ھ��
 * \����min         ���ֵ
 * \����max         ���ֵ
 * \����pagesize    ��Ӧ����¼�ʱÿҳ��С
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_HScrollbar_SetInfo(HWND hWnd, int min, int max, int pagesize);

/**
 * \��û�������Ϣ
 *
 * \����hWnd       ���ھ��
 * \����min        ���ֵ
 * \����max        ���ֵ
 * \����pagesize   ÿҳ��С
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_HScrollbar_GetInfo(HWND hWnd, int* min, int* max, int* pagesize);

/**
 * \���û������Ĳ���
 *
 * \����hWnd    ���ھ��
 * \����step    ����ֵ
 */
VOID SMP_HScrollbar_SetStep(HWND hWnd, int step);

/**
 * \���û���������λ��
 *
 * \����hWnd        ���ھ��
 * \����value       ��ֵ
 * \����redraw      �Ƿ��ػ洰��
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_HScrollbar_SetValue(HWND hWnd, int value, BOOL redraw);

/**
 * \�õ��������ĵ�ǰλ��
 *
 * \����hWnd        ���ھ��
 * \���ص�ǰλ��ֵ
 */
int SMP_HScrollbar_GetValue(HWND hWnd);

/**
 * \���ҹ���
 *
 * \����hWnd        ���ھ��
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_HScrollbar_PageLeft(HWND hWnd);

/**
 * \�������
 *
 * \����hWnd        ���ھ��
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_HScrollbar_PageRight(HWND hWnd);

/**
 * \���������ҹ���
 *
 * \����hWnd     ���ھ��
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_Scrollbar_LineRight(HWND hWnd);

/**
 * \�������������
 *
 * \����hWnd     ���ھ��
 * \�ɹ�����TRUE�����򷵻�FALSE
 */
BOOL SMP_Scrollbar_LineLeft(HWND hWnd);

/** 
���ڳ���

 * \�������Ĵ��ڹ���
 *
 * \����hWnd        ���ھ��
 * \����Msg         ������Ϣ
 * \����wParam      ��һ������
 * \����lParam      �ڶ�������
 * \������Ϣ�Ĵ�����
 */
LRESULT SMP_HScrollbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_HSCROLLBAR_H */

