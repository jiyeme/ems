#ifndef _TOPWND_H
#define _TOPWND_H

#include "window.h"

/**
 * \�������ڵ�ID����
 */
#define TOPWND_ID_START		300


/**
 * \���㴰��ID
 *
 * һ������IDӦ����һ����Ŀ
 * InitTopWindow implement.//InitTopWindowʵʩ
 */
enum {
	TOPWND_MAINWND = TOPWND_ID_START,
    TOPWND_OPENDLG,
    TOPWND_HELP,
    TOPWND_PREVIEW,
    TOPWND_CONVERT,
    TOPWND_START,
	//��������Ӵ��ڵ�ID
	MAX_TOPWND_COUNT //����
};

/**
 * \��ʼ����������
 *
 * �˺���Ӧ����InitApplication����
 */
VOID InitTopWindow(VOID);

/**
 * \��ȡ���㴰��
 *
 * \����wid	���ڵ�ID
 * \return 
 */
HWND GetTopWindow(WID wid);

/**
 * \��ʾһ����������
 *
 * \�������˴��ڵ�ID
 * \����listener��������listener
 * \�����������ݣ��û����������
 * \���ض������ھ����NULLʱʧ��
 */
HWND ShowTopWindow(WID wid, HWND listener, DWORD userdata);

/**
 * \����һ����������
 *
 * \����wid�����˴��ڵ�ID
 * \����destroy���Ƿ����ٴ���
 * \����redraw���Ƿ�ˢ�¶�������
 */
VOID HideTopWindow(WID wid, BOOL destroy, BOOL redraw);


#endif
