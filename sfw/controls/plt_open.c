#include "plt_open.h"
#include "mrc_exb.h"
#include "momo.h"
#include "mainwnd.h"
#include "mrc_base_i.h"

static HWND hOpenDlg;


/*��ȡϵͳ·��*/
static PSTR GetSysDir(void)
{
	PSTR dirname;

	dirname = (PSTR)mrc_malloc(40);
	mrc_memset(dirname, 0, 40);

	//mrc_GetCurrentDirectory(40, buf);
    mrc_strcpy(dirname, "c:/mythroad/");

	return dirname;
}

//����ƽ̨���ļ�
HWND Plt_Open(WID id, HWND hParent, HWND listener)
{
    PMR_T_FMGR_REQ pData;
	PSTR sysdir;

#ifdef WIN32
     return NULL;
#endif
    if(!hOpenDlg)
	    hOpenDlg = SGL_CreateWindow(Plt_Open_WndProc, 1, 1, 1, 1, id, WS_TRANSPARENT, 0);

 
    if(!hOpenDlg) return NULL;
    pData = _GET_WINDATA(hOpenDlg, PMR_T_FMGR_REQ);
	sysdir = GetSysDir();
    pData->dft_list = 0;//��list_path��·��Ϊ׼
	mrc_strcpy(pData->list_path, sysdir);
	mrc_free(sysdir);
	_LISTENER(hOpenDlg) = listener;
    SGL_DoModal(hOpenDlg, hParent);
    return hOpenDlg;
}

LRESULT Plt_Open_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	PMR_T_FMGR_REQ pData = _GET_WINDATA(hWnd, PMR_T_FMGR_REQ);

	switch(Msg)
	{
		case WM_CREATE:
		{
			pData = (PMR_T_FMGR_REQ)SGL_MALLOC(sizeof(MR_T_FMGR_REQ));
			if(!pData){
			//	SGL_TRACE("%s, %d: memory out\n", __FILE__, __LINE__);
				return 1;
			}
			SGL_MEMSET(pData, 0, sizeof(MR_T_FMGR_REQ));
			_SET_WINDATA(hWnd, pData);
            _SET_USERDATA(hWnd,0);
			break;
		}
		case WM_DESTROY:
		{
			if(pData) SGL_FREE(pData);
			break;
		}
		case WM_HIDE:
		{		
			break;
		}
		case WM_SHOW:
		{
	        uint8 type;

	        mrc_platEx(1401, NULL, 0, NULL, NULL, NULL); //��ʼ��˧ѡ��
	        type = MR_FMGR_TYPE_FOLDER;  //��ʾ�ļ���
	        mrc_platEx(1402, &type, sizeof(uint8), NULL, NULL, NULL); //����Ҫ��ʾ���ļ�����        	
	        type = MR_FMGR_TYPE_FOLDER_DOT;  //��ʾ�ļ���2
	        mrc_platEx(1402, &type, sizeof(uint8), NULL, NULL, NULL); //����Ҫ��ʾ���ļ�����        	
            type = MR_FMGR_TYPE_ALL; //��ʾ�ļ�
	        mrc_platEx(1402, &type, sizeof(uint8), NULL, NULL, NULL); //����Ҫ��ʾ���ļ�����
	        mrc_platEx(1404, (uint8*)pData, sizeof(MR_T_FMGR_REQ), NULL, NULL, NULL);//���ñ����ļ������
	        GAL_Flush();
			break;
		}
		case MR_LOCALUI_EVENT:
		{
			switch(wParam)
			{
			case MR_LOCALUI_KEY_OK:
                {
			        uint8* buf;
			        PSTR filename;
			        int32 namelen,len;
                    PSTR tt;

				    mrc_platEx(1405, NULL, 0, (uint8**)&buf, &namelen, NULL);
                    tt = GetSysDir();
                    len = mrc_strlen(tt);//Ϊ��ӦMOC,len = mrc_strlen(tt)+1;
                    mrc_free(tt);
				    filename = SGL_MALLOC(namelen+1);
				    mrc_memset(filename, 0, namelen+1);
				    mrc_strcpy(filename, (PSTR)(buf+len));
                    mrc_freeOrigin(buf, namelen);

				    mrc_platEx(1406, NULL, 0, NULL, NULL, NULL); //����
                    SGL_ExitModal(hWnd, MR_LOCALUI_KEY_OK, (LPARAM)filename);
                    SGL_FREE(filename);
                }
				break;
			case MR_LOCALUI_KEY_CANCEL:
				mrc_platEx(1406, NULL, 0, NULL, NULL, NULL); //����
                SGL_ExitModal(hWnd, MR_LOCALUI_KEY_CANCEL, 0);
				break;
			}
			break;
		}
	}
	return 0;
}

/**
 * �л�Ŀ¼����
 * ��������SwitchDisk
 * ������strDisk��ΪĿ¼�ַ���,��"A:/", "B:/"
 * ����ֵ��BOOL �ɹ���ʧ��
 */
BOOL SwitchDisk1(const char* strDisk)  //strDiskΪĿ¼�ַ���,��"A:/","B:/"
{
    int ret = MR_FAILED;
	char mrpname[128];
    unsigned char* output = NULL;
    int32 output_len = 0;

	/*�����л�Ŀ¼ǰmrp�ļ������л�Ŀ¼�󽫴Ӹ�Ŀ¼����MRP�ļ�*/
	mrc_memset(mrpname, 0, 80);
	//mrc_strcpy(mrpname,"mythroad/");
	mrc_strcat(mrpname, mrc_getPackName());

    #ifdef WIN32
         return FALSE;
    #endif
    ret = mrc_platEx(1204, (uint8*)strDisk, mrc_strlen(strDisk), &output, &output_len, NULL);
	//mrc_setPackName(mrpname); 

    return ret;
}
