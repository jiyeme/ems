#include "smp_progbar.h"


//Ĭ����С/���ֵ
#define PROGBAR_MIN_DEFAULT 	0
#define PROGBAR_MAX_DEFAULT	99


//����������
typedef struct SMP_ProgBarData
{
	Sint32 min;
	Sint32 max;
	Sint32 cur;
}PROGBARDATA, *PPROGBARDATA;

///////////////////////////////////////////////////////////////////////////////

VOID SMP_ProgBar_SetRange(HWND hWnd, Sint32 min, Sint32 max)
{
	PPROGBARDATA pData = _GET_WINDATA(hWnd, PPROGBARDATA);

	if(max < min) return;

	pData->min = min;
	pData->max = max;
	pData->cur = min;
}


VOID SMP_ProgBar_SetValue(HWND hWnd, Sint32 value, BOOL redraw, BOOL notify)
{
	PPROGBARDATA pData = _GET_WINDATA(hWnd, PPROGBARDATA);

// 	if(value < pData->min || value > pData->max || value == pData->cur)
// 		return;

	pData->cur = value;

	if(redraw)
		SGL_UpdateWindow(hWnd);

	if(notify)
		SGL_NotifyParent(hWnd, SMP_PROGBARN_VALUECHANGED, value);
}


LRESULT SMP_ProgBar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PPROGBARDATA pData = _GET_WINDATA(hWnd, PPROGBARDATA);

	switch(Msg)
	{
	
	case WM_CREATE:
	{
		pData = (PPROGBARDATA)SGL_MALLOC(sizeof(PROGBARDATA));
		if(!pData)
			return 1;
		

		SGL_MEMSET(pData, 0, sizeof(PROGBARDATA));
		pData->min = PROGBAR_MIN_DEFAULT;
		pData->max = PROGBAR_MAX_DEFAULT;
		pData->cur = PROGBAR_MIN_DEFAULT;
		
		_SET_WINDATA(hWnd, pData);
		break;
	}
	
	case WM_DESTROY:
	{
		if(pData) SGL_FREE(pData);
		break;
	}
		
	case WM_PAINT:
	{
		int x = 0, y = 0;
		SGL_WindowToScreen(hWnd, &x, &y);
		GAL_Rectangle(x, y, _WIDTH(hWnd), _HEIGHT(hWnd), COLOR_black);
		GAL_FillBox(x+1, y+1, DIV((pData->cur - pData->min) * (_WIDTH(hWnd) - 2), pData->max - pData->min) , _HEIGHT(hWnd) - 2, 0x0000a0);
		break;
	}
	
	}
	
	return 0;
}

