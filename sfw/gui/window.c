/*
** window.c: the implements of window.h
** 
** Copyright (C) 2007-2008 SKY-MOBI AS.  All rights reserved.
**
** Create date: 2007-12-27 by wandonglin
**
** This file is part of the simple gui library.
** It may not be redistributed under any circumstances.
*/

#include "window.h"
#include "rect.h"
#include "mrc_network.h"

//���Ե㣨x��y���Ƿ���hWnd������
#define PtInWnd(hWnd, x, y) \
	 (x >= _LEFT(hWnd) && x < _LEFT(hWnd) + _WIDTH(hWnd) \
	 && y >= _TOP(hWnd) && y < _TOP(hWnd) + _HEIGHT(hWnd))
/**
* \һ���ѿ������͵Ķ���
*/
typedef DWORD HBLOCKHEAP;


//��ǰ���㴰��
#define HTOPWND \
	desktop.top_window_list

//��ǰ�ĵ����˵�
#define HPOPUPMENU	\
	desktop.popup_menu

//��ǰģ̬����
#define HMODALDLG \
	desktop.modal_window

//��ǰ�������ٴ���
#define HMOUSETRACKER \
	desktop.mouse_tracker

//��ǰ��༭����
#define HACTIVEEDIT \
	desktop.hEdit

//��ǰ��༭����ƽ̨
#define HPLTEDIT \
	desktop.pltEdit

//��ǰӦ�ó�����¼�������
#define HAPPEVENTFILTER \
	desktop.evtfilter

//��ȡ�����б�

#define _FIRST_CHILD(hWnd) \
	((hWnd == HWND_DESKTOP)? HTOPWND : _CHILD(hWnd))

//��ȡ�����б�ο�
#define _CHILDREN_LIST(hWnd) \
	((hWnd == HWND_DESKTOP)? &HTOPWND : &_CHILD(hWnd))

//��������
///////////////////////////////////////////////////////////////////////////////////////////////

typedef struct SGL_Desktop
{
	HFONT system_font;					/* ��ǰϵͳ����*/
	HWND top_window_list;				/* ���㴰���б�*/
	Sint32 drawing_lock;	        	/* ��ͼ��ͣ��SGL_SuspendDrawing��������
                                        �Լ�SGL_UnsuspendDrawing������������������������ɶԵ��� */
	HMENU popup_menu;					/* ��ǰ�����˵�**/
	HWND modal_window;					/* ��ǰģ̬����*/
	HWND mouse_tracker;					/* ��ǰ�����ٴ���*/
	HWND hEdit;							/* ��ǰ�༭�򴰿ھ��*/
	int32 pltEdit;						/* ƽ̨�༭����*/
	SGL_AppEventFilter evtfilter;		/* Ӧ�ó����¼�������*/
#ifdef ENABLE_SGL_KEYREPEAT
	int32 hTimer;
	BOOL isRepeat;
#endif
}SGL_Desktop;

////////////////////////////////////////////////////////////////////////////////////////////
/**
* \��ʼ���ѿ顣
*
* \ע��bufferSize��blocksize����������4
* \�Լ�bufferSize����������blocksize
*
* \����buffer		���ݻ�������ָ��
* \����bufferSize	���ݻ������Ĵ�С
* \����blocksize	��Ĵ�С
* \return
*	- success	�Ѵ���ɹ�
*	- NULL		�Ѵ���ʧ��
*/
extern HBLOCKHEAP BlockHeap_Initialize(VOID* buffer, int buffersize, int blocksize);

/**
* \�Ӷѿ����һ����
*
* \����pHeap	�ѿ��ָ��
* \���ط���Ŀ��ַ�����ʧ���򷵻�NULL
*/
extern VOID*  BlockHeap_Alloc(HBLOCKHEAP* pHeap);

/**
* \�ͷŶѿ��е�һ����
*
* \����pHeap	�ѿ��ָ��
* \����pBlock	��ĵ�ַ
*/
extern VOID  BlockHeap_Free(HBLOCKHEAP* pHeap, VOID* pBlock);



static SGL_Desktop desktop;	//ֻ����һ������
static SGL_Window windows[SGL_MAX_WINDOW_COUNT]; //so winodw will not allow from global heap
static HBLOCKHEAP hWindowsHeap;

//����ʵ��
/////////////////////////////////////////////////////////////////////////////////////////////

/*
 * This macro is to help to apply command to all children in the children list.
 * ������ǰ�������������Ӵ������г����е��Ӵ���
 * \����list	�Ӵ����б�
 * \����first	��һ���Ӵ���
 *   to hold the current child the command can access this var\
     ����child  �ܱ��ֵ�ǰ�Ӵ��ڵ�������Է�ֹ�˷���
 * \����next        ��λ�ȡһ���Ӵ���
 * \����command     �����������Ӵ���
 */
#define SGL_MAP_ON_CHILDREN(list, first, child, next, command) {\
	if(NULL != (list)) \
	{\
		(child) = (first);\
		do{\
			{command;}\
		}while(((child) = (next)) != (first));\
	}\
}


static VOID
SGL_AddToWindowList(HWND* pHead, HWND hWnd)
{
	/*
	* here we not search the list before add the child window,����������޷���Ѱǰ����Ӵ��ڵ��б�
	* so user should ensure that the child window not exists in the list����û�Ӧȷ���Ӵ����б���û�д���
	*/
	
	if(*pHead)
	{
		//�ǿգ���ӵ�ͷ��
		_NEXT(hWnd) = *pHead;
		_PREV(hWnd) = _PREV(*pHead);		
		_NEXT(_PREV(*pHead)) = hWnd;
		_PREV(*pHead) = hWnd;
	}else{
		//�ձ�����Ϊ�����ڣ�next�����Լ�
		_NEXT(hWnd) = hWnd;
		_PREV(hWnd) = hWnd;
	}

	*pHead = hWnd;
}


static VOID
SGL_RemoveFromWindowList(HWND* pHead, HWND hWnd)
{
	//�Ӵ�����Z�����ĵ�һ����Ŀ
	if(hWnd == *pHead)
		*pHead = _NEXT(*pHead);
	
	if(hWnd == *pHead)
	{
		//ɾ�����Ӵ�����һ���Ӵ���
		*pHead = NULL;
	}else{
		//���Ӵ����б�ɾ��
		_NEXT(_PREV(hWnd)) = _NEXT(hWnd);
		_PREV(_NEXT(hWnd)) = _PREV(hWnd);
	}
	//�����Ӵ��ڵ�ָ��
	_PARENT(hWnd) = _NEXT(hWnd) = _PREV(hWnd) = NULL;
}


static VOID
SGL_BringToTopOfWindowList(HWND* pHead, HWND hWnd)
{
	//���Ӵ����б�ɾ��
	_NEXT(_PREV(hWnd)) = _NEXT(hWnd);
	_PREV(_NEXT(hWnd)) = _PREV(hWnd);

	//��ӵ��Ӵ����б�
	_NEXT(hWnd) = *pHead;
	_PREV(hWnd) = _PREV(*pHead);		
	_NEXT(_PREV(*pHead)) = hWnd;
	_PREV(*pHead) = hWnd;

	//����ͷָ�뵽�µĴ���
	*pHead = hWnd;
}


VOID GUIAPI
SGL_Initialize(VOID)
{
	desktop.system_font = (HFONT)MR_FONT_MEDIUM;

	//����Ĭ��
	desktop.drawing_lock = 0;
	desktop.top_window_list = NULL;

	hWindowsHeap = BlockHeap_Initialize(windows, sizeof(windows), sizeof(SGL_Window));
}


VOID GUIAPI
SGL_Terminate(VOID)
{
	//�ͷ�һЩ��Դ
    if(desktop.hTimer) mrc_timerDelete(desktop.hTimer);
	GAL_Terminate();
}


VOID GUIAPI
SGL_SuspendDrawing(VOID)
{
	++desktop.drawing_lock;
}


VOID GUIAPI
SGL_UnsuspendDrawing(VOID)
{
	--desktop.drawing_lock;
}


BOOL GUIAPI
SGL_IsSuspendDrawing(VOID)
{
	return desktop.drawing_lock > 0;
}


VOID GUIAPI
SGL_GetScreenSize(PSIZE pSize)
{
	if(!pSize) return;

	pSize->cx = GAL_Width();
	pSize->cy = GAL_Height();
}


HFONT GUIAPI
SGL_GetSystemFont(VOID)
{
	return desktop.system_font;
}


VOID GUIAPI
SGL_SetSystemFont(HFONT font)
{
	desktop.system_font = font;
}


HWND GUIAPI 
SGL_GetNextChild (HWND hWnd, HWND hChild)
{
	HWND hHead;
	if(NULL == hWnd || HWND_DESKTOP == hChild) 
		return NULL;

	hHead = _FIRST_CHILD(hWnd);
	if (hChild == NULL) 
		return hHead;

	if (hWnd != _PARENT(hChild)) 
		return NULL;

	return _NEXT(hChild) == hHead? NULL : _NEXT(hChild);
}


HWND GUIAPI
SGL_GetTopWindow(HWND hWnd)
{
	HWND hTopWnd = hWnd;
	
	while(IS_NORMAL_WINDOW(_PARENT(hTopWnd)) && !_IS_SET_ANY(hTopWnd, WS_MODAL))
		hTopWnd = _PARENT(hTopWnd);
	
	return hTopWnd;
}


HWND GUIAPI
SGL_FindChildWindow(HWND hWnd, WID id)
{
	HWND hChild, pHead;
	
	if(NULL == hWnd) return NULL;

	pHead = _FIRST_CHILD(hWnd);

	SGL_MAP_ON_CHILDREN
    (
        pHead, pHead, hChild, _NEXT(hChild), 
		if(_WID(hChild) == id) 
			return hChild
	);
		
	return NULL;
}


HWND GUIAPI
SGL_FindChildWindowByPosition(HWND hWnd, int x, int y)
{
	HWND hChild;

	if(NOT_IS_NORMAL_WINDOW(hWnd)) 
		return NULL;
	
	SGL_MAP_ON_CHILDREN(_CHILD(hWnd), _CHILD(hWnd), hChild, _NEXT(hChild), 
		if(PtInWnd(hChild, x, y)) 
			return hChild
	);
	
	return NULL;
}


VOID GUIAPI 
SGL_WindowToScreen(HWND hWnd, int* x, int* y)
{
	for(; IS_NORMAL_WINDOW(hWnd); hWnd = _PARENT(hWnd))
	{
		*x += _LEFT(hWnd);
		*y += _TOP(hWnd);
	}
}


VOID GUIAPI 
SGL_ScreenToWindow (HWND hWnd, int* x, int* y)
{
	for(; IS_NORMAL_WINDOW(hWnd); hWnd = _PARENT(hWnd))
	{
		*x -= _LEFT(hWnd);
		*y -= _TOP(hWnd);
	}
}


VOID GUIAPI
SGL_PaintWindowEx(HWND hWnd, PRECT pClipRect)
{
	HWND hChild;
	int x = 0, y = 0;
	RECT rchild, rclip;

	//���Ʊ���
	if(!_IS_SET_ANY(hWnd, WS_TRANSPARENT))
	{
		SGL_WindowToScreen(hWnd, &x, &y);
		GAL_FillBox(x + pClipRect->left, y + pClipRect->top, pClipRect->width, pClipRect->height, _BGCOLOR(hWnd));
	}

	//������Ϣ��hWnd�������ط����ʾ���ڱ��������Ӵ���
	if(_WNDPROC(hWnd)(hWnd, WM_PAINT, (WPARAM)pClipRect, 0))
		return;

	/* 
	* send message to children windows
	* acording to the z-order, we should traverse the children list in reverse order
	* and we have keep the last child window in the first child window's prev pointer

    *�Ӵ��ڵĴ��ڷ�����Ϣ
    *����z���������Ӧ�����෴��˳������Ӵ����б�
    *�����ǵĵ�һ���Ӵ������ڴ��ڵ�prevָ������һ���Ӵ���

    */
	SGL_MAP_ON_CHILDREN(_CHILD(hWnd), _PREV(_CHILD(hWnd)), hChild, _PREV(hChild), 
		_WINRECT(hChild, rchild);
		IntersectRect(&rclip, pClipRect, &rchild);
		if(rclip.width <= 0 || rclip.height <= 0) continue;
		rclip.left -= _LEFT(hChild);
		rclip.top -= _TOP(hChild);
		SGL_PaintWindowEx(hChild, &rclip)
	);
}


VOID GUIAPI
SGL_PaintWindow(HWND hWnd)
{
	HWND hChild;
	int x = 0, y = 0;

	//���Ʊ���
	if(!_IS_SET_ANY(hWnd, WS_TRANSPARENT))
	{
		SGL_WindowToScreen(hWnd, &x, &y);
		GAL_FillBox(x, y, _WIDTH(hWnd), _HEIGHT(hWnd), _BGCOLOR(hWnd));
	}

	//������Ϣ��hWnd�������ط����ʾ���ڱ��������Ӵ���
	if(_WNDPROC(hWnd)(hWnd, WM_PAINT, 0, 0))
		return;

	/* 
	* send message to children windows
	* acording to the z-order, we should traverse the children list in reverse order
	* and we have keep the last child window in the first child window's prev pointer
    * ������Ϣ���Ӵ���
    * ����z���������Ӧ�����෴��˳������Ӵ����б�
    * �Լ����ǵĵ�һ���Ӵ������ڴ��ڵ�prevָ������һ���Ӵ���
    */
	SGL_MAP_ON_CHILDREN(_CHILD(hWnd), _PREV(_CHILD(hWnd)), hChild, _PREV(hChild), 
		SGL_PaintWindow(hChild)
	);
}


static VOID
SGL_HideWindow(HWND hWnd)
{
	HWND hChild, hParent;

	_CLR_STYLE(hWnd, WS_VISIBLE);

	if(_IS_SET_ANY(hWnd, WS_INACTIVE))
	{
		SGL_SuspendDrawing(); //û�б�Ҫ�ػ�
		if(_IS_SET_ANY(hWnd, WS_POPUP))  //�����˵�����ʾ�ڶ��㴰�ڻ�һ��ģ̬����
			SGL_CloseMenu(HPOPUPMENU, FALSE);	
		else if(_IS_SET_ALL(hWnd, WS_MODAL | WS_OVERLAPPED))  //ģ̬���ڿ��Խ���ֻ�ڶ�����ʾ�Ĵ���

			SGL_ExitModal(HMODALDLG, 0,(LPARAM)HMODALDLG);
		SGL_UnsuspendDrawing();
	}
		
	if(HMOUSETRACKER && HMOUSETRACKER == hWnd)
		HMOUSETRACKER = NULL;

	hParent = _IS_SET_ANY(hWnd, WS_CHILD)? (HMODALDLG? HMODALDLG : HTOPWND) : hWnd;
	if(hParent && _FOCUS(hParent) && (_IS_SET_ANY(hWnd, WS_OVERLAPPED | WS_MODAL) || _FOCUS(hParent) == hWnd))
	{
		_CLR_STYLE(_FOCUS(hParent), WS_FOCUSED);
		SGL_SuspendDrawing(); //û�б�Ҫ�ػ�

		SGL_SendMessage(_FOCUS(hParent), WM_KILLFOCUS, 0, 0); 
		SGL_UnsuspendDrawing();
		if(!_IS_SET_ALL(hWnd, WS_OVERLAPPED | WS_KEEPFOCUS))
			_FOCUS(hParent) = NULL;
	}
		
	/*
	 *here we send message to parent window first,
	 *then broadcast the message to all children. 
	 *for reasons that in the WM_SHOW parent window may adjust children position/size

    *��������Ƿ�����Ϣ�������ڵĵ�һ��
    *Ȼ��㲥��Ϣ�������Ӵ��ڡ�
    *ԭ����WM_SHOW�����ڿ��ܻ�����Ӵ��ڵ�λ��/��С

    */
	_WNDPROC(hWnd)(hWnd, WM_HIDE, 0, 0);	

	SGL_MAP_ON_CHILDREN(_CHILD(hWnd), _CHILD(hWnd), hChild, _NEXT(hChild), 
		SGL_HideWindow(hChild)
	);
}


static VOID
SGL_ShowWindow(HWND hWnd)
{
	HWND hChild, hFocus;

	_SET_STYLE(hWnd, WS_VISIBLE);

	/*
	 *here we send message to parent window first,
	 *then broadcast the message to all children. 
	 *for reasons that in the WM_SHOW parent window may adjust children position/size
	 
    *��������Ƿ�����Ϣ�������ڵĵ�һ��
    *Ȼ��㲥��Ϣ�������Ӵ��ڡ�
    *ԭ����WM_SHOW�����ڿ��ܻ�����Ӵ��ڵ�λ��/��С

    */
	_WNDPROC(hWnd)(hWnd, WM_SHOW, 0, 0);	

	SGL_MAP_ON_CHILDREN(_CHILD(hWnd), _CHILD(hWnd), hChild, _NEXT(hChild), 
		SGL_ShowWindow(hChild)
	);

	if(_IS_SET_ALL(hWnd, WS_OVERLAPPED | WS_KEEPFOCUS) 
		&& _FOCUS(hWnd) && !_IS_SET_ANY(_FOCUS(hWnd), WS_FOCUSED))
	{
		hFocus = _FOCUS(hWnd);	//���ֽ���Ĵ���
		_FOCUS(hWnd) = NULL;	//����ɵĽ���
		SGL_SuspendDrawing();
		SGL_SetFocusWindow(hWnd, hFocus);
		SGL_UnsuspendDrawing();
	}else if(_IS_SET_ANY(hWnd, WS_OVERLAPPED | WS_MODAL)){
		SGL_SuspendDrawing();
		SGL_SendMessage(hWnd, WM_INITFOCUS, 0, 0);
		SGL_UnsuspendDrawing();
	}
}

#ifndef _EXT_RELEASE
//static int swcount;
#endif

HWND GUIAPI
SGL_CreateWindow(SGL_WndProc wndproc, int x, int y, int w, int h, WID id, DWORD style, DWORD userdata)
{
	HWND hWnd = NULL;
	
	//������
	if(!wndproc || w <= 0 || h <= 0)
		return NULL;
	
	if( (hWnd = (HWND)BlockHeap_Alloc(&hWindowsHeap)) == NULL )
		return NULL;

	SGL_MEMSET(hWnd, 0, sizeof(SGL_Window));
	_WID(hWnd)			= id;
	_LEFT(hWnd)			= x;
	_TOP(hWnd)			= y;
	_WIDTH(hWnd)		= w;
	_HEIGHT(hWnd)		= h;
	_STYLE(hWnd)		= style;
	_WNDPROC(hWnd)		= wndproc;
	_USERDATA(hWnd)		= userdata;
	_FGCOLOR(hWnd)		= COLOR_black;
	_BGCOLOR(hWnd)		= COLOR_lightwhite;

	//����WM_CREATE��Ϣ
	if(SGL_SendMessage(hWnd, WM_CREATE, 0, 0))
	{
		SGL_DestroyWindow(hWnd);
		return NULL;
	}

#ifndef _EXT_RELEASE
//	SGL_TRACE("%s, %d: Create Window: %d, %d\n", __FILE__, __LINE__, hWnd->id, ++swcount);
#endif
	return hWnd;
}


BOOL GUIAPI
SGL_DestroyWindow(HWND hWnd)
{
	if(NOT_IS_NORMAL_WINDOW(hWnd))
		return FALSE;
	
#ifndef _EXT_RELEASE	
//	SGL_TRACE("%s, %d: Destroy Window %d, %d\n", __FILE__, __LINE__, hWnd->id, --swcount);
#endif

	//ɾ���Ӵ����б�
	SGL_RemoveChildWindow(hWnd);

	SGL_SendMessage(hWnd, WM_DESTROY, 0, 0);

	//�������е��Ӵ���
	while(_CHILD(hWnd)) 
		SGL_DestroyWindow(_CHILD(hWnd));
		
	BlockHeap_Free(&hWindowsHeap, hWnd);
	return TRUE;
}


/*
 * �˺���ֻ���� ģ̬/���� ����!!!!!!!!!
 */
static BOOL
SGL_AddChildWindowEx(HWND hParent, HWND hChild, DWORD type)
{
	if(NOT_IS_NORMAL_WINDOW(hParent)
		|| NOT_IS_NORMAL_WINDOW(hChild)
		|| NULL != _PARENT(hChild))
		return FALSE;

	_PARENT(hChild) = hParent;
	_SET_STYLE(hChild, type);
	
	SGL_AddToWindowList(&_CHILD(hParent), hChild);
	if(SGL_IsWindowVisible(hParent))
	{
		SGL_ShowWindow(hChild);
		SGL_UpdateWindow(hChild);
	}	

	return TRUE;
}


BOOL GUIAPI
SGL_AddChildWindow(HWND hParent, HWND hChild)
{
	if(NULL == hParent 
		|| NOT_IS_NORMAL_WINDOW(hChild)
		|| NULL != _PARENT(hChild))
		return FALSE;

	//��ӵ��Ӵ����б�
	_PARENT(hChild) = hParent;
	if(HWND_DESKTOP == hParent)
	{
		//���һ�����㴰��
		if(IS_NORMAL_WINDOW(HTOPWND))
			SGL_HideWindow(HTOPWND);

		_SET_STYLE(hChild, WS_OVERLAPPED);
		SGL_AddToWindowList(&HTOPWND, hChild);
		SGL_ShowWindow(hChild);
		SGL_UpdateWindow(hChild);
	}
    else
    {
		//���һ���Ӵ���
		_SET_STYLE(hChild, WS_CHILD);
		SGL_AddToWindowList(&_CHILD(hParent), hChild);
		if(SGL_IsWindowVisible(hParent))
		{
			SGL_ShowWindow(hChild);
			SGL_UpdateWindow(hChild);
		}
	}

	return TRUE;
}


BOOL GUIAPI
SGL_RemoveChildWindow(HWND hChild)
{
	HWND hParent;
	
	if( NOT_IS_NORMAL_WINDOW(hChild)
		|| NULL == _PARENT(hChild))
		return FALSE;

	hParent = _PARENT(hChild);
	SGL_RemoveFromWindowList(_CHILDREN_LIST(hParent), hChild);

	//before remove the window from the window list, send message first
	//�ڴ����б���ɾ���Ĵ��ڣ����͵�һ����Ϣ
	if(SGL_IsWindowVisible(hChild))
	{		
		SGL_HideWindow(hChild);

		//�������ĸ�����
		if(HWND_DESKTOP != hParent)
		{
			RECT rchild;
			_WINRECT(hChild, rchild);			
			SGL_UpdateWindowEx(hParent, &rchild); //�������ǿ���ֻ����ɾ���Ӵ��ھ���
		}else if(HTOPWND)
        {
			SGL_ShowWindow(HTOPWND);
			SGL_UpdateWindow(HTOPWND);
		}
	}
	
	return TRUE;
}


BOOL GUIAPI
SGL_BringWindowToTop(HWND hWnd)
{
	HWND *pHead;
	
	if(NOT_IS_NORMAL_WINDOW(hWnd) || _PARENT(hWnd) == NULL)
		return FALSE;

	//�Ѿ���������Ĵ���
	pHead =_CHILDREN_LIST(_PARENT(hWnd));
	if(*pHead == hWnd)
		return FALSE;

	//����������Ϣ���ɵĶ��㴰��
	if(_PARENT(hWnd) == HWND_DESKTOP)
		SGL_HideWindow(HTOPWND);

	SGL_BringToTopOfWindowList(pHead, hWnd);

	//�������´����Ƿ�ɼ��ģ�������WM_PAINT��Ϣ
	if(_PARENT(hWnd) == HWND_DESKTOP)
		SGL_ShowWindow(hWnd);

	SGL_UpdateWindow(hWnd);
	return TRUE;
}


LRESULT GUIAPI
SGL_SendMessage(HWND hWnd, UINT Msg, DWORD wParam, DWORD lParam)
{	
	if(NULL == hWnd || HWND_DESKTOP == hWnd)
		return 0;

	return hWnd->wndproc(hWnd, Msg, wParam, lParam);
}


VOID GUIAPI
SGL_BroadcastMessageEx(HWND hWnd, UINT Msg, DWORD wParam, DWORD lParam, HWND hExcept)
{
	HWND hChild;

	if(NOT_IS_NORMAL_WINDOW(hWnd) || hWnd == hExcept) 
		return;
	
	if(_WNDPROC(hWnd)(hWnd, Msg, wParam, lParam))
		return;
	
	SGL_MAP_ON_CHILDREN(_CHILD(hWnd), _CHILD(hWnd), hChild, _NEXT(hChild), 
		SGL_BroadcastMessageEx(hChild, Msg, wParam, lParam, hExcept)
	);
}


//this fuction for supporting scroll view
static HWND
SGL_GetClipWindow(HWND hWnd)
{
	for(; IS_NORMAL_WINDOW(hWnd); hWnd = _PARENT(hWnd))
	{
		if(_PARENT(hWnd) == HWND_DESKTOP)
			return NULL;
		
		if(_TOP(hWnd) < 0 || _LEFT(hWnd) < 0 
			|| _TOP(hWnd) + _HEIGHT(hWnd) > _HEIGHT(_PARENT(hWnd))
			|| _LEFT(hWnd) + _WIDTH(hWnd) > _WIDTH(_PARENT(hWnd)))
			return _PARENT(hWnd);
	}

	return NULL;
}


VOID GUIAPI
SGL_UpdateWindowEx(HWND hWnd, PRECT pClipRect)
{
	int x=0, y=0;
	RECT rChild, rParent, rClip; 
	HWND hClipWnd;
	
	if(SGL_IsSuspendDrawing() || !SGL_IsWindowVisible(hWnd))
		return;

	if(NULL != (hClipWnd = SGL_GetClipWindow(hWnd)))
	{
		//֧�ֹ���
		_WINRECT(hClipWnd, rParent);
		SGL_WindowToScreen(_PARENT(hClipWnd), &rParent.left, &rParent.top);

		if(pClipRect)
		{
			rChild = *pClipRect;
			SGL_WindowToScreen(hWnd, &rChild.left, &rChild.top);
		}else{
			_WINRECT(hWnd, rChild);
			SGL_WindowToScreen(_PARENT(hWnd), &rChild.left, &rChild.top);
		}
		
		IntersectRect(&rClip, &rChild, &rParent);
		if(rClip.left != rChild.left || rClip.top != rChild.top 
			|| rClip.width != rChild.width || rClip.height != rChild.height)
		{
			if(rClip.width <= 0 || rClip.height <= 0)
				return;

			//��Ҫʱ���ǿ���ʹ��SGL_PaintWindowEx
			rChild = rClip; // transfer the screen ���仭��
			SGL_ScreenToWindow(hWnd, &rChild.left, &rChild.top);
			SGL_PaintWindowEx(hWnd, &rChild);
		}else{
			SGL_PaintWindow(hWnd);
		}

		GAL_FlushRegion(rClip.left, rClip.top, rClip.width, rClip.height);
	}else{
		if(pClipRect)
		{
			SGL_PaintWindowEx(hWnd, pClipRect);
			SGL_WindowToScreen(hWnd, &x, &y);
			GAL_FlushRegion(x+pClipRect->left, y+pClipRect->top, pClipRect->width, pClipRect->height);
		}else{
			SGL_PaintWindow(hWnd);
			SGL_WindowToScreen(hWnd, &x, &y);	
			GAL_FlushRegion(x, y, _WIDTH(hWnd), _HEIGHT(hWnd));
		}
	}
}
VOID GUIAPI
SGL_ClrFocusWindow(HWND hWnd)
{
    _CLR_STYLE(_FOCUS(hWnd), WS_FOCUSED);
    SGL_SendMessage(_FOCUS(hWnd), WM_KILLFOCUS, 0, 0);
    _FOCUS(hWnd)=NULL;
}


VOID GUIAPI
SGL_SetFocusWindow(HWND hParent, HWND hChild)
{
	HWND hOldActive = _FOCUS(hParent);

	if(hChild == hOldActive 
		|| !(IS_NORMAL_WINDOW(hChild) && _IS_SET_ANY(hChild, WS_CHILD))
		|| ! _IS_SET_ANY(hParent, WS_OVERLAPPED | WS_MODAL))
		return;

	//����ɵĽ���
	if(IS_NORMAL_WINDOW(hOldActive) && _IS_SET_ANY(hOldActive, WS_FOCUSED))
	{
		_CLR_STYLE(hOldActive, WS_FOCUSED);
		SGL_SendMessage(hOldActive, WM_KILLFOCUS, 0, 0);
	}

	//����hchildΪ����
	_FOCUS(hParent) = hChild;
	_SET_STYLE(hChild, WS_FOCUSED);
	SGL_SendMessage(hChild, WM_SETFOCUS, 0, 0);
}


HWND GUIAPI
SGL_GetFocusWindow(HWND hWnd)
{
	if(IS_NORMAL_WINDOW(hWnd) 
		&& _IS_SET_ANY(hWnd, WS_OVERLAPPED | WS_MODAL))
	{
		if(_FOCUS(hWnd) && _IS_SET_ANY(_FOCUS(hWnd), WS_FOCUSED))
			return _FOCUS(hWnd);
		
		return hWnd;
	}

	return HTOPWND;
}


HWND GUIAPI
SGL_FindFocusNext(HWND hHead, HWND hEnd, BOOL next)
{
	HWND hFocus, hFirst, hChild = hHead;

	do{
		if(_IS_SET_ANY(hChild, WS_TABSTOP))
		{
			if(!_IS_SET_ANY(hChild, WS_FOCUSCHILD))
				return hChild;

			if(_CHILD(hChild))
			{
				hFirst = next? _PREV(_CHILD(hChild)) : _CHILD(hChild);
				if((hFocus = SGL_FindFocusNext(hFirst, hFirst, next)) != NULL)
					return hFocus;
			}
		}
		hChild = next? _PREV(hChild) : _NEXT(hChild);
	}while(hChild != hEnd);

	return NULL;
}


HWND GUIAPI
SGL_FindFocus4TopWnd(HWND hWnd, BOOL next)
{
	HWND hParent, hChild, hHead, hEnd, hFirst;
	HWND hFocus = NULL;
	
	if(NOT_IS_NORMAL_WINDOW(hWnd) 
		|| !_CHILD(hWnd)
		|| ! _IS_SET_ANY(hWnd, WS_OVERLAPPED | WS_MODAL))
		return NULL;
	
	if(_FOCUS(hWnd))
	{
		hChild = _FOCUS(hWnd);		
		do{
			hParent = _PARENT(hChild);
			hHead = next? _PREV(hChild) : _NEXT(hChild);
			hEnd = hParent == hWnd? hChild : (next? _PREV(_CHILD(hParent)) : _CHILD(hParent));
			
			//��������齹���Ӵ����Ƿ������һ������
			if(hHead != hEnd 
				&& (hFocus = SGL_FindFocusNext(hHead, hEnd, next)) != NULL)
				break; 
		}while((hChild = hParent) != hWnd);
	} 

	if(NULL == _FOCUS(hWnd) || NULL == hFocus)
	{
		hFirst = next? _PREV(_CHILD(hWnd)) : _CHILD(hWnd);
		hFocus = SGL_FindFocusNext(hFirst, hFirst, next); 
	}

	return hFocus;
}


VOID GUIAPI
SGL_FocusNext(HWND hWnd, BOOL next)
{
	HWND hFocus = SGL_FindFocus4TopWnd(hWnd, next);
	if(hFocus)
		SGL_SetFocusWindow(hWnd, hFocus);
}


VOID GUIAPI
SGL_SetMouseTracker(HWND hWnd)
{
	HMOUSETRACKER = hWnd;
}


HWND GUIAPI
SGL_GetMouseTracker(VOID)
{
	return HMOUSETRACKER;
}


static LRESULT 
SGL_HandleMouseEvents(HWND hWnd, UINT Msg, int x, int y)
{
	BOOL found; //һ����־�������˳�����
	HWND hChild = _CHILD(hWnd);

	//�ҵ�Ŀ�괰��
	while(_CHILD(hWnd))
	{
		hChild = _CHILD(hWnd);
		found = FALSE; 
		
		//���Ӵ����б��в���
		do{
			if(PtInWnd(hChild, x, y))
			{
				x -= _LEFT(hChild);
				y -= _TOP(hChild);
				hWnd = hChild;
				found = TRUE;
				break;
			}
		}while((hChild = _NEXT(hChild)) != _CHILD(hWnd));

		//��û�����Ӵ����б����ҵ���Ӧ������
		if(!found) break;
	}

	return SGL_SendMessage(hWnd, Msg, x, y);
}


static VOID
SGL_HandleMenuMouseEvents(UINT Msg, int x, int y)
{
	HMENU hMenu = NULL, hSub = NULL;
	HWND hParent = _PARENT(HPOPUPMENU);

	SGL_ScreenToWindow(hParent, &x, &y);
	
	//�˴��и������������Ӳ˵���������
	hSub = (HWND)SGL_SendMessage(HPOPUPMENU, WM_GETSUBMENU, (DWORD)x, (DWORD)y);
	if(SGL_IsWindowVisible(hSub) && PtInWnd(hSub, x, y)){
		hMenu = hSub;
	}else if(PtInWnd(HPOPUPMENU, x, y)){
		hMenu = HPOPUPMENU;
	}
	
	if(Msg == WM_MOUSEDOWN)
	{
	    if(!hMenu)
	    {
		    //SGL_SendMessage(hParent, WM_MENU_MOUSEDOWN, x, y);
		    return;
	    }
// 		SGL_HandleMouseEvents(hMenu, Msg, x - _LEFT(hMenu), y - _TOP(hMenu)); //����ò˵������Ӳ˵�����
		SGL_SendMessage(hMenu, Msg, x - _LEFT(hMenu), y - _TOP(hMenu));
	}
    else if(Msg == WM_MOUSEUP)
    {
		if(hMenu)
        {
// 			SGL_HandleMouseEvents(hMenu, Msg, x - _LEFT(hMenu), y - _TOP(hMenu)); //����ò˵������Ӳ˵�����
			SGL_SendMessage(hMenu, Msg, x - _LEFT(hMenu), y - _TOP(hMenu));
		}
		else if(!SGL_SendMessage(hParent, WM_MENUMOUSEUP, x, y))
			SGL_CloseMenu(HPOPUPMENU, TRUE);
	}else
    {
		//����ƶ�
	}
}


#ifdef ENABLE_SGL_KEYREPEAT		

static VOID SGL_KeyRepeatCallback(int32 data)
{
	SGL_DispatchEvents(WM_KEYDOWNREPEAT, data, 0);
}


static VOID SGL_KeyRepeatStartCallback(int32 data)
{
	mrc_timerStart(desktop.hTimer, SGL_KEYREPEAT_INTERVAL, data, SGL_KeyRepeatCallback, TRUE);
}


VOID GUIAPI SGL_StopKeyRepeat(VOID)
{
	if(desktop.isRepeat)
	{
		mrc_timerStop(desktop.hTimer);
		desktop.isRepeat = FALSE;
	}
}

#endif


VOID GUIAPI SGL_DispatchEvents(UINT Msg, WPARAM wParam, LPARAM lParam)
{
	HWND hActive;

#ifdef ENABLE_SGL_KEYREPEAT		
	//����������¼�ʱ������Ӧֹͣ
	if(desktop.isRepeat && Msg != WM_KEYDOWNREPEAT)
		SGL_StopKeyRepeat();
#endif	

	//�¼��ѱ�����
	if(HAPPEVENTFILTER && HAPPEVENTFILTER(Msg, wParam, lParam))
		return;
		
	switch(Msg)
	{

	//��������¼�
	case WM_MOUSEDOWN:
	case WM_MOUSEUP:
	case WM_MOUSEMOVE:
	{
		if(HMOUSETRACKER)
		{
			SGL_ScreenToWindow(HMOUSETRACKER, (int*)&wParam, (int*)&lParam);
			SGL_HandleMouseEvents(HMOUSETRACKER, Msg, (int)wParam, (int)lParam);
			if(Msg == WM_MOUSEUP)
				HMOUSETRACKER = NULL;
			return;
		}

		if(HPOPUPMENU)
		{
			SGL_HandleMenuMouseEvents(Msg, (int)wParam, (int)lParam);
		}else if(HMODALDLG){
			//���Ǻ����˶Ի���ָ��
			int x = wParam, y = lParam;
			SGL_ScreenToWindow(HMODALDLG, &x, &y);
			if(x >= 0 && x < _WIDTH(HMODALDLG) 
				&& y >= 0 && y < _HEIGHT(HMODALDLG))
				SGL_HandleMouseEvents(HMODALDLG, Msg, x, y);
			else
				SGL_SendMessage(HTOPWND, WM_MOUSEDOWN == Msg? WM_MODALMOUSEDOWN : WM_MODALMOUSEUP, wParam, lParam);
		}else if(HTOPWND){
			SGL_HandleMouseEvents(HTOPWND, Msg, wParam, lParam);
		}	
		break;
	}

	//������¼�
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_KEYDOWNREPEAT:
	{		
#ifdef ENABLE_SGL_KEYREPEAT		
		if(Msg == WM_KEYDOWN)
		{
			if(!desktop.hTimer)
				desktop.hTimer = mrc_timerCreate();
			mrc_timerStart(desktop.hTimer, SGL_KEYREPEAT_START_INTERVAL, wParam, SGL_KeyRepeatStartCallback, FALSE);
			desktop.isRepeat = TRUE;
		}else if(Msg == WM_KEYUP){
			SGL_StopKeyRepeat();
		}
#endif

		if(HPOPUPMENU)
		{
			/*
			 * �������˵������ݲ˵��е������¼�
			 * �����ط��㣬�رղ˵�
			 */
			if(SGL_SendMessage(HPOPUPMENU, Msg, wParam, lParam))
				SGL_CloseMenu(HPOPUPMENU, TRUE);
		}else if(HMODALDLG){
			/*
			 * ��ģ̬���ڣ����ݴ��ڵ������¼�
			 * �Ի�Ӧ���ֽ����Ӵ��ڣ�����������....
		 	 * ����ֻ�Ǵ��ݵ�ģ̬���ڣ���Ҫʱ���
			 */
			hActive = SGL_GetFocusWindow(HMODALDLG);
			do{
				if(SGL_SendMessage(hActive, Msg, wParam, lParam) || hActive == HMODALDLG)
					return;				
			}while(NULL != (hActive = _PARENT(hActive)));
		}else{
			/* 
			 * ���͹ؼ���Ϣ������Ӵ��ڣ�
			 * �����¼���������ʱ�����¼��������Ӵ��ڴ���
			 */
			hActive = SGL_GetFocusWindow(HTOPWND);
			while(IS_NORMAL_WINDOW(hActive) && !SGL_SendMessage(hActive, Msg, wParam, lParam))
				hActive = _PARENT(hActive);
		}

		break;
	}

	case MR_DIALOG_EVENT:
	{
		if(HACTIVEEDIT && HPLTEDIT > 0)
		{
			if(wParam == MR_DIALOG_KEY_OK)
			{
				PCWSTR pText = (PCWSTR)mrc_editGetText(HPLTEDIT);	
				SGL_SendMessage(HACTIVEEDIT, WM_SETEDITTEXT, (WPARAM)pText, 0);
			}

			mrc_editRelease(HPLTEDIT);
			SGL_UnsuspendDrawing();
			SGL_UpdateWindow(HTOPWND);					
			HPLTEDIT = 0;
			HACTIVEEDIT = NULL;
			break;
		}

		//break ͨ��
	}

	default:
		//�����¼�
		SGL_SendMessage(HMODALDLG? HMODALDLG : HTOPWND, Msg, wParam, lParam);				
		break;
	}
}


LRESULT GUIAPI
SGL_Window_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	// TODO:
	return 0;
}


BOOL GUIAPI
SGL_PopupMenu(HMENU hMenu, int x, int y, HWND hParent)
{
	if(NOT_IS_NORMAL_WINDOW(hMenu) 
		|| !SGL_IsWindowVisible(hParent)
		|| !_IS_SET_ANY(hParent, WS_OVERLAPPED | WS_MODAL)
		|| _IS_SET_ANY(hParent, WS_INACTIVE)
		|| HPOPUPMENU)
		return FALSE;

	_LEFT(hMenu) = x;
	_TOP(hMenu) = y;
	
	if(SGL_AddChildWindowEx(hParent, hMenu, WS_POPUP))
	{
		_SET_STYLE(hParent, WS_INACTIVE | WS_POPUP);
		HPOPUPMENU = hMenu;
		SGL_SendMessage(hParent, WM_MENUSHOW, 0, (LPARAM)hMenu);
	}
	
	return HPOPUPMENU == hMenu;
}


VOID GUIAPI
SGL_SelectMenuItem(HMENU hMenu, WID item, DWORD userdata)
{
	HWND hParent;

	if(NOT_IS_NORMAL_WINDOW(HPOPUPMENU) 
		|| !SGL_IsWindowVisible(hMenu))
		return;

	hParent = _PARENT(hMenu);
	SGL_CloseMenu(HPOPUPMENU, FALSE);
	SGL_SendNotifyMessage((_LISTENER(hMenu)? _LISTENER(hMenu) : hParent), _WID(hMenu), item, userdata);
}


HMENU GUIAPI
SGL_GetPopupMenu(VOID)
{
	return HPOPUPMENU;
}


VOID GUIAPI
SGL_CloseMenu(HMENU hMenu, BOOL redraw)
{
	HWND hParent = _PARENT(hMenu);

	if(NOT_IS_NORMAL_WINDOW(hMenu) 
		|| hMenu != HPOPUPMENU)
		return;

	_CLR_STYLE(hParent, WS_INACTIVE |WS_POPUP);
	HPOPUPMENU = NULL;	
	
	SGL_SuspendDrawing();
	SGL_RemoveChildWindow(hMenu);	
	SGL_SendMessage(hParent, WM_MENUHIDE, (WPARAM)redraw, (LPARAM)hMenu);
	SGL_UnsuspendDrawing();

	//hMenu must not be destroied in message WM_MENUHIDE//hMenu���벻����WM_MENUHIDE��Ϣ������
	if(redraw || !_IS_SET_ANY(hMenu, WS_NOREDRAW))		
		SGL_UpdateWindow(HTOPWND);
}


BOOL GUIAPI
SGL_DoModal(HWND hDlg, HWND hParent)
{
	BOOL update;
	
	if(NOT_IS_NORMAL_WINDOW(hDlg)//����hDlg�Ƿ���ģ̬���ڡ����Ը������Ƿ���ӡ��������Ƿ�Ϊ��������
		|| !SGL_IsWindowVisible(hParent)
		|| !_IS_SET_ANY(hParent, WS_OVERLAPPED))
		return FALSE;

	if(HMODALDLG)
	{
		/*
		* ���ģ̬���ڵ���ʽ��WS_NOREDRAW��
		* SGL_CloseMenu������¸����ڵĲ˵����������ǵ���SGL_UpdateWindow��
		*/
		update = _IS_SET_ANY(HMODALDLG, WS_NOREDRAW);
		SGL_ExitModal(HMODALDLG, 0, (LPARAM)HMODALDLG);
		if(update) SGL_UpdateWindow(hParent);
	}
    else if(HPOPUPMENU)
    {
		/*
		* ����˵�����ʽ��WS_NOREDRAW��
		* SGL_CloseMenu������¸����ڵĲ˵����������ǵ���SGL_UpdateWindow��
		*/
		update = _IS_SET_ANY(HPOPUPMENU, WS_NOREDRAW);
		SGL_CloseMenu(HPOPUPMENU, FALSE);
		if(update) SGL_UpdateWindow(hParent);
	} 

	if(SGL_AddChildWindowEx(hParent, hDlg, WS_MODAL))
	{
		_SET_STYLE(hParent, WS_INACTIVE | WS_MODAL);		
		HMODALDLG = hDlg;
		SGL_SendMessage(hParent, WM_MODALSHOW, 0, (LPARAM)hDlg);
		return TRUE;
	}
	
	return FALSE;
}


BOOL GUIAPI
SGL_ExitModal(HWND hDlg, WORD code, LPARAM lParam)
{
	HWND hParent;
	BOOL redraw;
	
	if(hDlg == NULL
		|| hDlg != HMODALDLG)
		return FALSE;

	hParent = _PARENT(hDlg);
	HMODALDLG = NULL;
	redraw = !_IS_SET_ANY(hDlg, WS_NOREDRAW);

	_CLR_STYLE(hParent, WS_INACTIVE | WS_MODAL);
	
	SGL_SuspendDrawing();
	SGL_RemoveChildWindow(hDlg);
	SGL_SendMessage(hParent, WM_MODALHIDE, 0, (LPARAM)hDlg);
	SGL_SendNotifyMessage((_LISTENER(hDlg)? _LISTENER(hDlg) : hParent), _WID(hDlg), code, lParam);
	SGL_UnsuspendDrawing();

	/*
	* the dialog window should not be destroied in message WM_MODALHIDE, 
	* but maybe the dialog has be destroied in the notify message, 
	* so here we save the value in redraw
	/ *
	* �Ի����ڲ�Ӧ����WM_MODALHIDE��Ϣ�����٣�
	* Ҳ�п����ǶԻ��������ƻ��Ѿ���֪ͨ��Ϣ��
	* �����������Ǳ����ػ���ֵ
	*/
	if(redraw)
		SGL_UpdateWindow(HTOPWND);
	
	return TRUE;
}


HWND GUIAPI
SGL_GetModalWindow(VOID)
{
	return HMODALDLG;
}


VOID GUIAPI
SGL_TextInput(HWND hEdit)
{
	SGL_EditInfo info = {0, };
	int32 type = MR_EDIT_ANY;

	SGL_SendMessage(hEdit, WM_GETEDITINFO, (WPARAM)&info, 0);
	if(!info.buffer || info.size == 0)
		return;

	HACTIVEEDIT = hEdit;

	if(_IS_SET_ANY(hEdit, ES_NUM)) 
		type = MR_EDIT_NUMERIC;
	else if(_IS_SET_ANY(hEdit, ES_PWD)) 
		type = MR_EDIT_PASSWORD;
	else if(_IS_SET_ANY(hEdit, ES_ALPHA))
		type = MR_EDIT_ANY;

	SGL_SuspendDrawing();
	HPLTEDIT = mrc_editCreate((PCSTR)info.title, (PCSTR)info.buffer, type, DIV(info.size, 2) - 1);
}


LRESULT GUIAPI
SGL_SetAppEventFilter(SGL_AppEventFilter filter)
{
	SGL_AppEventFilter old = HAPPEVENTFILTER;

	HAPPEVENTFILTER = filter;
	return (LRESULT)old;
}


