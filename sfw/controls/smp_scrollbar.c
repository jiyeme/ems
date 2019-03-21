#include "smp.h"
#include "smp_scrollbar.h"

#define SCRBAR_MIN_DEFAULT 	    0
#define SCRBAR_MAX_DEFAULT	    99
#define SCRBAR_STEP_DEFAULT	    1
#define SCRBAR_PAGE_DEFAULT	(SCRBAR_MAX_DEFAULT - SCRBAR_MIN_DEFAULT + 1) / 10

#define SCRBAR_SMP_HEADER_HEIGHT	12
#define MIN_CURSOR_HEIGHT		8

#define HAVE_BUTTON 0 //�Ƿ��ð�ť0��1û��

//�ڲ�ʹ��
#define GET_SCRBAR_DATA(hWnd) \
	_GET_WINDATA(hWnd, PSBDATA)

#define SET_SCRBAR_DATA(hWnd, data) \
	_SET_WINDATA(hWnd, data)


///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct SMP_ScrBarData
{
	int min;     //��Сֵ
	int max;     //���ֵ
	int cur;     //��ǰλ��
	int page;    //ҳ���
	int step;    //�й��
}SBDATA, * PSBDATA;

/////////////////////////////////////////////////////////////////////////////////////////////

//������ȡ�����Ϣ
static VOID SMP_Scrollbar_GetCursorInfo(PSBDATA pScrbar, int h, int* cursor_y, int* cursor_h)
{
	//������λ��
	int total = MAX(pScrbar->max - pScrbar->min + 1, pScrbar->page);
	int maxSteps = pScrbar->max - pScrbar->min + 1 - pScrbar->page;

	*cursor_h = (h*pScrbar->page)/total; 
	if(*cursor_h < MIN_CURSOR_HEIGHT) *cursor_h = MIN_CURSOR_HEIGHT;
	
	h -= *cursor_h;
	if(maxSteps < 1) maxSteps = 1; 	
	*cursor_y = (h* (pScrbar->cur - pScrbar->min)) / maxSteps;

	//���߶��ڲ˵������һ����ù���ƶ����������ĵײ�
	if(pScrbar->cur == pScrbar->max) 
		*cursor_y = h;
}


BOOL SMP_Scrollbar_SetInfo(HWND hWnd, int min, int max, int pagesize)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd) 
		|| pagesize < 0 || min > max)
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	pScrbar->min = min;
	pScrbar->max = max;
	pScrbar->page = pagesize;
	return TRUE;
}


BOOL SMP_Scrollbar_GetInfo(HWND hWnd, int* min, int* max, int* pagesize)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	if(min) *min = pScrbar->min;
	if(max) *max = pScrbar->max;
	if(pagesize) *pagesize = pScrbar->page;
	return TRUE;
}


VOID SMP_Scrollbar_SetStep(HWND hWnd, int step)
{
	PSBDATA pScrbar = GET_SCRBAR_DATA(hWnd);
	pScrbar->step = step;
}


BOOL SMP_Scrollbar_SetValue(HWND hWnd, int index, BOOL redraw)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	if(index == pScrbar->cur || index < pScrbar->min || index > MAX(pScrbar->max - pScrbar->page + 1, 0))
		return FALSE;

	pScrbar->cur = index;
	if(redraw)
	{
		SGL_UpdateWindow(hWnd);
		//֪ͨ������λ�øı�
		SGL_NotifyParent(hWnd, SMP_SCRBARN_VALUECHANGED, index);
	}
	return TRUE;
}


int SMP_Scrollbar_GetValue(HWND hWnd)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return 0;
	
	pScrbar = GET_SCRBAR_DATA(hWnd);
	return pScrbar->cur;
}


BOOL SMP_Scrollbar_PageUp(HWND hWnd)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	if(pScrbar->cur > pScrbar->min)
    {
		if(SMP_Scrollbar_SetValue(hWnd, MAX(pScrbar->cur - pScrbar->page, pScrbar->min), TRUE))
            return SMP_SCRBARN_LAST;
        else
	        return FALSE;
    }
	return TRUE;
}

BOOL SMP_Scrollbar_PageDown(HWND hWnd)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	if(pScrbar->cur < pScrbar->max - pScrbar->page + 1)
    {
		if(SMP_Scrollbar_SetValue(hWnd, MIN( pScrbar->cur + pScrbar->page, pScrbar->max - pScrbar->page + 1), TRUE))
            return SMP_SCRBARN_LAST;
        else
	        return FALSE;
    }
	return TRUE;
}


BOOL SMP_Scrollbar_LineUp(HWND hWnd)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	if(pScrbar->cur > pScrbar->min)
    {
        if(SMP_Scrollbar_SetValue(hWnd, MAX(pScrbar->cur - pScrbar->step, pScrbar->min), TRUE))
            return SMP_SCRBARN_LAST;
        else
            return FALSE;
    }
	return TRUE;
}


BOOL SMP_Scrollbar_LineDown(HWND hWnd)
{
	PSBDATA pScrbar;

	if(!IS_NORMAL_WINDOW(hWnd))
		return FALSE;

	pScrbar = GET_SCRBAR_DATA(hWnd);
	if(pScrbar->cur < pScrbar->max - pScrbar->page + 1)
    {
        if(SMP_Scrollbar_SetValue(hWnd, MIN(pScrbar->cur + pScrbar->step, pScrbar->max - pScrbar->page + 1), TRUE))
            return SMP_SCRBARN_LAST;
        else
            return FALSE;
    }
	return TRUE;
}


LRESULT SMP_Scrollbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    PSBDATA pScrbar = GET_SCRBAR_DATA(hWnd);

	switch(Msg)
	{
	case WM_CREATE:
	{
		pScrbar = SGL_MALLOC(sizeof(SBDATA));
		if(!pScrbar)
			return 1;
		SGL_MEMSET(pScrbar, 0, sizeof(SBDATA));
		pScrbar->cur = pScrbar->min = SCRBAR_MIN_DEFAULT;
		pScrbar->max = SCRBAR_MAX_DEFAULT;
		pScrbar->page = SCRBAR_PAGE_DEFAULT;
		pScrbar->step = SCRBAR_STEP_DEFAULT; // Ĭ�ϲ���

		SET_SCRBAR_DATA(hWnd, pScrbar);
		_BGCOLOR(hWnd) = 0;
		_FGCOLOR(hWnd) = COLOR_controlhili;
		return 0;
	}

	case WM_DESTROY:
	{
		if(pScrbar) SGL_FREE(pScrbar);
		return 0;
	}

	case WM_PAINT:
	{
		int x=0, y=0, cursor_y=0, cursor_h=0;

		SGL_WindowToScreen(hWnd, &x, &y);

#if HAVE_BUTTON
		SMP_Scrollbar_GetCursorInfo(pScrbar, _HEIGHT(hWnd) - 2*SCRBAR_SMP_HEADER_HEIGHT, &cursor_y, &cursor_h);

        //������ť���
		GAL_FillBox(PHYSICALGC, x + 1, y + 1, _WIDTH(hWnd)-2, SCRBAR_SMP_HEADER_HEIGHT-2, COLOR_controlbg);
		//������ť�߿�
        GAL_Rectangle(PHYSICALGC, x, y, _WIDTH(hWnd), SCRBAR_SMP_HEADER_HEIGHT, _FGCOLOR(hWnd));
		//�ײ���ť���
        GAL_FillBox(PHYSICALGC, x+1, y + _HEIGHT(hWnd) - SCRBAR_SMP_HEADER_HEIGHT +1, _WIDTH(hWnd)-2, SCRBAR_SMP_HEADER_HEIGHT-2, COLOR_controlbg);
		//�ײ���ť�߿�
        GAL_Rectangle(PHYSICALGC, x, y + _HEIGHT(hWnd) - SCRBAR_SMP_HEADER_HEIGHT, _WIDTH(hWnd), SCRBAR_SMP_HEADER_HEIGHT, _FGCOLOR(hWnd));
        //������ť������
		GAL_Line(x + 2, y + SCRBAR_SMP_HEADER_HEIGHT - 4, x + DIV(_WIDTH(hWnd), 2) - 1, y + 4, COLOR_darkgray);//���
		GAL_Line(x + _WIDTH(hWnd) - 4, y + SCRBAR_SMP_HEADER_HEIGHT - 4, x + DIV(_WIDTH(hWnd), 2) - 1, y + 4, COLOR_darkgray);//�ұ�
		//�ײ���ť������
		GAL_Line(x + 2, y + _HEIGHT(hWnd) - SCRBAR_SMP_HEADER_HEIGHT + 4, x + DIV(_WIDTH(hWnd), 2) - 1, y + _HEIGHT(hWnd) - 4, COLOR_darkgray);//���
		GAL_Line(x + _WIDTH(hWnd) - 4, y + _HEIGHT(hWnd) - SCRBAR_SMP_HEADER_HEIGHT + 4, x + DIV(_WIDTH(hWnd), 2) - 1, y + _HEIGHT(hWnd) - 4, COLOR_darkgray);//�ұ�
		//�����
		GAL_FillBox(PHYSICALGC, x + 1, y + SCRBAR_SMP_HEADER_HEIGHT + cursor_y, _WIDTH(hWnd) - 2, cursor_h, COLOR_controlbg);
        //�߿�
		GAL_Rectangle(PHYSICALGC, x, y + SCRBAR_SMP_HEADER_HEIGHT + cursor_y, _WIDTH(hWnd), cursor_h, _FGCOLOR(hWnd));
#else
        SMP_Scrollbar_GetCursorInfo(pScrbar, _HEIGHT(hWnd), &cursor_y, &cursor_h);
		//�����
        #define SCROLLBARCOLOR 0x002888D0
        y += cursor_y;
        GAL_Line(x+1,y,x+2,y,SCROLLBARCOLOR);
		GAL_FillBox(x, y+1,_WIDTH(hWnd), cursor_h-2, SCROLLBARCOLOR);
        y+=cursor_h-1;
        GAL_Line(x+1,y,x+2,y,SCROLLBARCOLOR);
#endif
		return 0;
	}

	case WM_MOUSEDOWN:
	{
		int cursor_y, cursor_h;

#if HAVE_BUTTON
		if((int)lParam < SCRBAR_SMP_HEADER_HEIGHT)
			SMP_Scrollbar_LineUp(hWnd);
		else if((int)lParam > _HEIGHT(hWnd) - SCRBAR_SMP_HEADER_HEIGHT)
			SMP_Scrollbar_LineDown(hWnd);
		else
        {
#endif


			SMP_Scrollbar_GetCursorInfo(pScrbar, _HEIGHT(hWnd), &cursor_y, &cursor_h);
			if((int)lParam < cursor_y)
				SMP_Scrollbar_PageUp(hWnd);
			else if((int)lParam > cursor_y + cursor_h)
				SMP_Scrollbar_PageDown(hWnd);


#if  HAVE_BUTTON
		}
#endif
		return 0;
	}

	case WM_KEYDOWN:
	case WM_KEYDOWNREPEAT:
	{
		UINT key = (UINT) wParam;		
		if(key == MR_KEY_DOWN)
		{
			SMP_Scrollbar_LineDown(hWnd);
			return 1;
		}
        else if(key == MR_KEY_UP)
        {
			SMP_Scrollbar_LineUp(hWnd);
			return 1;
		}
		break;
	}
	
	}
	return 0;
}


