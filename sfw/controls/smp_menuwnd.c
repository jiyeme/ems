#include "i18n.h"
#include "smp.h"
#include "smp_menuwnd.h"
#include "smp_toolbar.h"
#include "smp_edit.h"
#include "mainwnd.h"
#include "smp_label.h"

static DWORD sSavedIDs[SMP_TOOLBAR_ITEMCOUNT];
static PCWSTR sSavedStr[SMP_TOOLBAR_ITEMCOUNT];

LRESULT SMP_MenuWnd_WndProc(HWND hToolbar, HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{	
	switch(Msg)
	{

	case WM_MODALSHOW:
	{
		//ֹͣ����༭����˸
		if(_FOCUS(hWnd) && _WNDPROC(_FOCUS(hWnd)) == SMP_Edit_WndProc)
			SMP_Edit_StopBlink(_FOCUS(hWnd), FALSE);
		break;
	}
	case WM_MODALHIDE:
	{	
		//ʹ����༭��˸
		if(_FOCUS(hWnd) && _WNDPROC(_FOCUS(hWnd)) == SMP_Edit_WndProc)
			SMP_Edit_StartBlink(_FOCUS(hWnd), FALSE);		
		break;
	}
	case WM_MENUSHOW:
	{
		//ֹͣ����༭��˸
		if(_FOCUS(hWnd) && _WNDPROC(_FOCUS(hWnd)) == SMP_Edit_WndProc)
			SMP_Edit_StopBlink(_FOCUS(hWnd), FALSE);

		//���ù��������ַ���Ϊ��ѡ�񡱺͡�ȡ����
		if(SMP_Toolbar_StrIsID(hToolbar))
        {
		    sSavedIDs[SMP_TOOLBARLOC_LEFT] = SMP_Toolbar_GetString(hToolbar, SMP_TOOLBARLOC_LEFT);
		    sSavedIDs[SMP_TOOLBARLOC_MID] = SMP_Toolbar_GetString(hToolbar, SMP_TOOLBARLOC_MID);
		    sSavedIDs[SMP_TOOLBARLOC_RIGHT] = SMP_Toolbar_GetString(hToolbar, SMP_TOOLBARLOC_RIGHT);
            SMP_Toolbar_SetStrings(hToolbar, STR_SELECT, RESID_INVALID, STR_CANCEL, TRUE);
        }
        else
        {
		    sSavedStr[SMP_TOOLBARLOC_LEFT] = SMP_Toolbar_GetString2(hToolbar, SMP_TOOLBARLOC_LEFT);
		    sSavedStr[SMP_TOOLBARLOC_MID] = SMP_Toolbar_GetString2(hToolbar, SMP_TOOLBARLOC_MID);
		    sSavedStr[SMP_TOOLBARLOC_RIGHT] = SMP_Toolbar_GetString2(hToolbar, SMP_TOOLBARLOC_RIGHT);
            SMP_Toolbar_SetStrings2(hToolbar, SGL_LoadString(STR_SELECT), NULL, SGL_LoadString(STR_CANCEL), TRUE);
        }

		break;
	}
	case WM_MENUHIDE:
	{	
		//ʹ�༭������˸
		if(_FOCUS(hWnd) && _WNDPROC(_FOCUS(hWnd)) == SMP_Edit_WndProc)
			SMP_Edit_StartBlink(_FOCUS(hWnd), FALSE);

		//�ָ����������ַ���
        if(SMP_Toolbar_StrIsID(hToolbar))
        {
		    SMP_Toolbar_SetStrings(hToolbar, sSavedIDs[SMP_TOOLBARLOC_LEFT], sSavedIDs[SMP_TOOLBARLOC_MID], sSavedIDs[SMP_TOOLBARLOC_RIGHT], FALSE);
        }
        else
        {
		    SMP_Toolbar_SetStrings2(hToolbar, sSavedStr[SMP_TOOLBARLOC_LEFT], sSavedStr[SMP_TOOLBARLOC_MID], sSavedStr[SMP_TOOLBARLOC_RIGHT], FALSE);
        }
		break;
	}
	case WM_MENUMOUSEUP:
	{
		if((int)lParam < _HEIGHT(hWnd) - SMP_ITEM_HEIGHT)
			return 0;
		
		//the scrollbar will set as the mouse tracker on MOUSE DOWN, so we should reset the mouse tracker
	    //����������������갴��������������������Ӧ������������
		SGL_SendMessage(hToolbar, WM_MOUSEDOWN, wParam - _LEFT(hToolbar), lParam - _TOP(hToolbar));
		SGL_SendMessage(hToolbar, WM_MOUSEUP, wParam - _LEFT(hToolbar), lParam - _TOP(hToolbar));
		SGL_SetMouseTracker(NULL);
		return 1;
	}
	case WM_MENUMOUSEDOWN:
	{
		//NOT IMPLEMENTED YET/��δʵ��
		break;
	}
	
	}
	return 0;
}