#ifndef _PLT_OPEN_FILE
#define _PLT_OPEN_FILE

#include "window.h"


/*
  ���������

  \code
  	//ֻ��Ҫ���������������ʾPlat_Open
  	Plat_Open(...);

  	//��Ӧ��Ϣ��֪ͨ
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the Plat_Open id")
		{
            PSTR path = (PSTR)lParam;

			if(code == MR_LOCALUI_KEY_OK)
			{
				//�������
			}else{    //MR_LOCALUI_KEY_CANCEL
				//����ȡ��
			}
		}	
  \endcode
*/

HWND Plt_Open(WID id, HWND hParent, HWND listener);


LRESULT Plt_Open_WndProc(HWND hDlg, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif