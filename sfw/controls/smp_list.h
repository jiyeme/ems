#ifndef _SMP_LIST_H
#define _SMP_LIST_H

#include "window.h"

/**
  \�б�ؼ�

  Ҫʹ���б�ؼ�:
  - ����������ݻص�
  - �����б�ؼ�
  - �����б����ݻص�
  - ���ӵ�������
  - ��Ӧ��Ϣ

  \code
  int YourListGetTotal(VOID)//�б������ص�
  {
  	...
  }

  VOID YourListGetRowData(int index, PSMPROWDATA pRowData)//��ȡ�б�������
  {
  	...
  }

  //�����б�ؼ�
  hList = SGL_CreateWindow(SMP_List_WndProc, ...);
  SMP_List_SetDataProvider(hList, YourListGetTotal, YourListGetRowData);
  SGL_AddChildWindow(hWnd, hList);

  //��Ӧ֪ͨ��Ϣ
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the list id")
		{
			int index = (int)lParam; //ѡ���е�λ��
			switch(code)
			{
			case SMP_LISTN_HILICHANGED:
				break;
			case SMP_LISTN_SELECTED:
				break;
			case SMP_LISTN_CLICKED:
				break;
			}
		}	  
  \endcode
*/


//�б�û�й�����
#define SMP_LISTS_NOSCRBAR			0x0001L


/**
 * \�����иı�ʱ����
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_HILICHANGED)
 *		{
 *			int index = (int)lParam;
 * 			//֪ͨ��Ϣ���
 *		}
 * \endcode
 *
 * \����index  ��ǰ����λ��
 */
#define SMP_LISTN_HILICHANGED			0x0001

/**
 * \��������ʱ����
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_SELECTED)
 *		{
 *			int index = (int)lParam;
 * 			//֪ͨ��Ϣ���
 *		}
 * \endcode
 *
 * \����index  ��ǰ����λ��
 */
#define SMP_LISTN_SELECTED			0x0002

/**
 * \��������ʱ����
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_CLICKED)
 *		{
 *			int index = (int)lParam;
 * 			//֪ͨ��Ϣ���
 *		}
 * \endcode
 *
 * \����index  ��ǰ����λ��
 */
#define SMP_LISTN_CLICKED				0x0003



/**
 * \Max supported collumns.  
 */
#define SMP_LIST_MAX_STR_COL		2

//����Ϣ
typedef struct SMP_RowData
{	
	Uint16 margin;          /*����*/	
	Uint16 colWidth0;       /*����ͼ����*/	
	uint16 colHeight0;      /*����ͼ��߶� */	
	HBITMAP hBmp;           /*ͼ����*/	
	struct{
		PCWSTR str;         /*���ַ�*/		
		Uint16 width;       /*���ַ���ʾ���*/
	}cols[SMP_LIST_MAX_STR_COL];

}SMPROWDATA, *PSMPROWDATA;

/**
 * \�ص�����,��ȡ������
 * \�����б�������
 */
typedef int (*SMP_List_GetTotal)(VOID);

/**
 * \�ص�����,��ȡ������
 *
 * \���� index            ��λ��
 * \����[out] pRowData    ������
 */
typedef VOID (*SMP_List_GetRowData)(int index, PSMPROWDATA pRowData);

/**
 * \���г���
 *
 * \���� index		��λ��
 * \���� r			��������
 * \���� pRowData	������
 * \���� hilight	������λ��
 * \���� flush		�Ƿ�ˢ��
 */
typedef VOID (*SMP_List_DrawRow)(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush);


/**
 * ����list�ؼ��Ļ�ȡ���ݺͻ�ȡ�����Ļص�����
 * 
 * ���list�ؼ���һ���GUIʵ�ֵ�list�ؼ�������ͬ
 * ���������洢list������, ����ͨ�������ص���������ȡ�ܹ������к�ÿ�е���ϸ��Ϣ
 * ����ʹ������Ҫ����list�ؼ������ص�����
 * ����:
 * hWnd:			list�ؼ��ľ��
 * fnGetTotal:		��ȡ�б��ܹ������Ļص�����
 * fnGetRowData:	��ȡ�б�ÿ�е����ݵĻص�����(��0��ʼ)
 */
VOID SMP_List_SetDataProvider(HWND hList, SMP_List_GetTotal fnGetTotal, SMP_List_GetRowData fnGetRowData);

/**
 * \����һ�����⻭�г���
 *
 * \���� hList			���ھ��
 * \���� fnDrawRow		�»��г���
 */
VOID SMP_List_SetDrawRowCallback(HWND hList, SMP_List_DrawRow fnDrawRow);

/**
 * \Ĭ�ϻ��к���.
 *
 * \���� index			�б�����
 * \���� r				�����б��еľ�������
 * \���� pRowData		������
 * \���� hilight		��ǰ����������
 * \���� flush          �Ƿ�ˢ��
 */
VOID SMP_List_DefaultDrawRowCallback(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush);

/**
 * \ͨ������ı������.
 *
 * \����hList     ���ھ��
 * \����index     ��λ��
 */
VOID SMP_List_HilightByIndex(HWND hList, int index);

/**
 * \��ȡ������λ��
 *
 * \����hList	�б��ھ��
 * \������λ��
 */
int SMP_List_GetHilightIndex(HWND hList);

/**
 * \��ȡ��ǰҳ��ʼ��λ�ú�ÿҳ�Ĵ�С
 *
 * \����hList				�б��ھ��
 * \����[out] pagestart		��ǰҳ��һ������
 * \����[out] pagesize		ÿҳ��ʾ������
 */
VOID SMP_List_GetPageInfo(HWND hList, int* pagestart, int* pagesize);

/**
 * \�Ϸ�ҳ
 *
 * \���� hList     ���ھ��
 */
VOID SMP_List_PageUp(HWND hList);

/**
 * \�·�ҳ
 *
 * \���� hList     ���ھ��
 */
VOID SMP_List_PageDown(HWND hList);

/**
 * \�Ϸ���
 *
 * \���� hList     ���ھ��
 */
VOID SMP_List_LineUp(HWND hList);

/**
 * \�·���
 *
 * \���� hList     ���ھ��
 */
VOID SMP_List_LineDown(HWND hList);

/**
 * \֪ͨ�б�,�������ı�
 *
 * \����hList		���ھ��
 * ˢ��list�ؼ�, ��list�����������仯�Ժ����
 */
VOID SMP_List_Refresh(HWND hList);


/**
 * \�����б�
 * 
 * \���� hList     ���ھ��
 * ����ص���һ��
 */
VOID SMP_List_Reset(HWND hList);


//���ڹ���
LRESULT SMP_List_WndProc(HWND hList, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_LIST_H */

