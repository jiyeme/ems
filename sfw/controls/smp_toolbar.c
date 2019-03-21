#include "i18n.h"
#include "bmp.h"
#include "smp.h"
#include "smp_toolbar.h"
#include "momo.h"
#include "myadv.h"

typedef struct SMP_ToolbarData
{
	DWORD ids[SMP_TOOLBAR_ITEMCOUNT];
	PCWSTR str[SMP_TOOLBAR_ITEMCOUNT];
	int32 width[SMP_TOOLBAR_ITEMCOUNT];
	int32 height;
    BOOL StrType; //��������ID��ʽ(TRUE)�������ַ���ָ����ʽ(FALSE)
}TBDATA, *PTBDATA;

//////////////////////////////////////////////////////////////////////////////////////////////

VOID SMP_Toolbar_SetStrings(HWND hWnd, DWORD left, DWORD mid, DWORD right, BOOL redraw)
{
	int i; 
    int32 h = 0;
	HFONT font = SGL_GetSystemFont();
	PTBDATA	pTbData = _GET_WINDATA(hWnd, PTBDATA);

	pTbData->ids[SMP_TOOLBARLOC_LEFT] = left;
	pTbData->ids[SMP_TOOLBARLOC_MID] = mid;
	pTbData->ids[SMP_TOOLBARLOC_RIGHT] = right;
	pTbData->str[SMP_TOOLBARLOC_LEFT] = SGL_LoadString(left);
	pTbData->str[SMP_TOOLBARLOC_MID] = SGL_LoadString(mid);	
	pTbData->str[SMP_TOOLBARLOC_RIGHT] = SGL_LoadString(right);
	pTbData->height = 0;
    pTbData->StrType = TRUE;

	for(i = 0; i < SMP_TOOLBAR_ITEMCOUNT; i++)
	{
		if(pTbData->str[i])
        {
            if(enable_smallfont)
                skyfontWidthHeight((PSTR)pTbData->str[i],&pTbData->width[i], &h);
            else
                mrc_textWidthHeight((PSTR)pTbData->str[i], TRUE, (uint16)font, &pTbData->width[i], &h);
        }

		if(h > pTbData->height) pTbData->height = h;
	}

	if(redraw && SGL_IsWindowVisible(hWnd))
		SGL_UpdateWindow(hWnd);
}

VOID SMP_Toolbar_SetStrings2(HWND hWnd, PCWSTR left, PCWSTR mid, PCWSTR right, BOOL redraw)
{
	int i; 
    int32 h = 0;
    skyfont_screenRect_t rect = {0,0,-1,-1};
	HFONT font = SGL_GetSystemFont();


	PTBDATA	pTbData = _GET_WINDATA(hWnd, PTBDATA);
	pTbData->ids[SMP_TOOLBARLOC_LEFT] = RESID_INVALID;
	pTbData->ids[SMP_TOOLBARLOC_MID] = RESID_INVALID;
	pTbData->ids[SMP_TOOLBARLOC_RIGHT] = RESID_INVALID;
	pTbData->str[SMP_TOOLBARLOC_LEFT] = left;
	pTbData->str[SMP_TOOLBARLOC_MID] = mid;
	pTbData->str[SMP_TOOLBARLOC_RIGHT] = right;
	pTbData->height = 0;
    pTbData->StrType = FALSE;

	for(i = 0; i < SMP_TOOLBAR_ITEMCOUNT; i++)
	{
		if(pTbData->str[i])
        {
            if(enable_smallfont)
                mrc_skyfont_textWidthHeight((PSTR)pTbData->str[i],0,-1,0,0,rect,0,&pTbData->width[i], &h);
            else
                mrc_textWidthHeight((PSTR)pTbData->str[i], TRUE, (uint16)font, &pTbData->width[i], &h);

        }
		if(h > pTbData->height) pTbData->height = h;
	}

	if(redraw && SGL_IsWindowVisible(hWnd))
		SGL_UpdateWindow(hWnd);
}
BOOL SMP_Toolbar_StrIsID(HWND hWnd)
{
	PTBDATA	pTbData = _GET_WINDATA(hWnd, PTBDATA);
	return pTbData->StrType;
}
DWORD SMP_Toolbar_GetString(HWND hWnd, int location)
{
	PTBDATA	pTbData = _GET_WINDATA(hWnd, PTBDATA);
	return pTbData->ids[location];
}

PCWSTR SMP_Toolbar_GetString2(HWND hWnd, int location)
{
	PTBDATA	pTbData = _GET_WINDATA(hWnd, PTBDATA);
	return pTbData->str[location];
}

LRESULT SMP_Toolbar_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PTBDATA pTbData = _GET_WINDATA(hWnd, PTBDATA);
	
	switch(Msg) 
	{
	
	case WM_CREATE:
	{
		pTbData = (PTBDATA)SGL_MALLOC(sizeof(TBDATA));
		if(!pTbData)
			return 1;
		
		
		SGL_MEMSET(pTbData, 0, sizeof(TBDATA));
		_SET_WINDATA(hWnd, pTbData);
		_SET_STYLE(hWnd, WS_TRANSPARENT);
// 		_BGCOLOR(hWnd) = 0; //0x3383cc;
// 		_FGCOLOR(hWnd) = COLOR_lightwhite;		
		return 0;
	}

	case WM_DESTROY:
	{
		if(pTbData) SGL_FREE(pTbData);
		return 0;
	}

	case WM_SHOW:
	{
		_CLR_STYLE(hWnd, WS_PRESSED);
		break;
	}

	case WM_PAINT:
	{
		int x = _LEFT(hWnd);
		int y = _TOP(hWnd),t;
		HFONT font = SGL_GetSystemFont();
        mr_colourSt clr;
        skyfont_screenRect_t rect1;

		SGL_WindowToScreen(_PARENT(hWnd), &x, &y);	
        clr.r = PIXEL888RED(_FGCOLOR(hWnd)), clr.g = PIXEL888GREEN(_FGCOLOR(hWnd)), clr.b = PIXEL888BLUE(_FGCOLOR(hWnd));
        rect1.x=x,rect1.y=y,rect1.w=_WIDTH(hWnd),rect1.h=_HEIGHT(hWnd);

		//����
        t=_HEIGHT(hWnd)/3;
        GAL_DrawHLine(x,y,SCREEN_WIDTH,0xB6BCCC);
		DrawShadeRect(x,y,SCREEN_WIDTH,t,0xffffff,0xE5EAF5,SHADE_UPDOWN);
        DrawShadeRect(x,y+t,SCREEN_WIDTH,_HEIGHT(hWnd)-t,0xD4DBED,0xE1E6F6,SHADE_UPDOWN);

        y += DIV(_HEIGHT(hWnd) - pTbData->height, 2);
		if(pTbData->str[0])
        {
            if(enable_smallfont)
                mrc_skyfont_drawTextLeft((PSTR)pTbData->str[0], 0, -1, (int16)(x + SMP_ITEM_MARGIN),(int16)y, rect1, clr, 0);
            else
                mrc_drawText((PSTR)pTbData->str[0], 
                    (int16)(x + SMP_ITEM_MARGIN),(int16)y, 
                    clr.r, clr.g, clr.b, TRUE, (uint16)font);
		}
		if(pTbData->str[1])
        { 
            if(enable_smallfont)
                mrc_skyfont_drawTextLeft((PSTR)pTbData->str[1], 0, -1, (int16)(x + DIV((_WIDTH(hWnd) - pTbData->width[1]), 2)),(int16)y, rect1, clr, 0);
            else
                mrc_drawText((PSTR)pTbData->str[1], 
                    (int16)(x + DIV((_WIDTH(hWnd) - pTbData->width[1]), 2)),(int16)y, 
                    clr.r, clr.g, clr.b, TRUE, (uint16)font);
        }
		if(pTbData->str[2])
		{
            if(enable_smallfont)
                mrc_skyfont_drawTextLeft((PSTR)pTbData->str[2], 0, -1, (int16)(SCREEN_WIDTH - pTbData->width[2] - SMP_ITEM_MARGIN),(int16)y, rect1, clr, 0);
            else
			    mrc_drawText((PSTR)pTbData->str[2],
				    (int16)(SCREEN_WIDTH - pTbData->width[2] - SMP_ITEM_MARGIN),(int16)y, 
				    clr.r, clr.g, clr.b, TRUE, (uint16)font);
		}
#ifdef ENABLE_ADV

        {//���ù��λ��
            int16 advx,advw;

            advw = (int16)(_WIDTH(hWnd)-(pTbData->width[0]+pTbData->width[2])-2*SMP_ITEM_MARGIN)-20;
            advx = x + DIV((_WIDTH(hWnd) - advw), 2);
            advSetRect(advx,y,advw,FONT_H);
        }
#endif
		return 0;
	}

	case WM_MOUSEDOWN:
	{
		_SET_STYLE(hWnd, WS_PRESSED);
		SGL_SetMouseTracker(hWnd);
		return 0;
	}

	case WM_MOUSEUP:
	{
		int x, i;

		if(!_IS_SET_ANY(hWnd, WS_PRESSED))
			return 0;

		_CLR_STYLE(hWnd, WS_PRESSED);
		for(i = 0; i < SMP_TOOLBAR_ITEMCOUNT; i++)
		{
			x = DIV(
                (_WIDTH(hWnd) - pTbData->width[i])*i
                , 2);
			if(pTbData->str[i] 
				&& *pTbData->str[i] 
				&& (int)wParam >= x 
				&& (int)wParam <= (x + pTbData->width[i]))
			{
				SGL_DispatchEvents(WM_KEYUP
					, (i == SMP_TOOLBARLOC_LEFT? MR_KEY_SOFTLEFT : (i==SMP_TOOLBARLOC_MID? MR_KEY_SELECT : MR_KEY_SOFTRIGHT))
					, 0);
				break;
			}
		}		
		return 0;
	}
	
	}

	return 0;
}

