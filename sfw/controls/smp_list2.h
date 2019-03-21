#ifndef _SMP_LIST_H
#define _SMP_LIST_H

#include "window.h"

/**
  \ defgroup smp_list���б�

  ��Ҫʹ�ü��б�
   - ������������ṩ�ص�
   - �������б�
   - �������Ļص��б������ṩ��
   - ��ӵ�������
   - �¼���Ӧ��֪ͨ

  \code
  int YourListGetTotal(VOID)
  {
  	...
  }

  VOID YourListGetRowData(int index, PSMPROWDATA pRowData)
  {
  	...
  }

  //�����б�
  hList = SGL_CreateWindow(SMP_List_WndProc, ...);
  SMP_List_SetDataProvider(hList, YourListGetTotal, YourListGetRowData);
  SGL_AddChildWindow(hWnd, hList);

  //��Ӧ��Ϣ��֪ͨ
	case WM_COMMAND:
		WID id = LOWORD(wParam);
		WORD code = HIWORD(wParam);

		if(id == "the list id")
		{
			int index = (int)lParam; //ѡ���е�����
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
  

  

���ƴ�����ʽ

��ʾ�б�û�й�������
*/
#define SMP_LISTS_NOSCRBAR			0x0001L

/*
����NOTIFY��Ϣ
 * \brief Sent when high light index changed. \�߹�ʱ���͵�ָ���仯
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_HILICHANGED)
 *		{
 *			int index = (int)lParam;
 * 			//�����֪ͨ��Ϣ

 *		}
 * \endcode
 *
 * \param index current high light index \����ָ��Ŀǰ�ĸ߹�ָ��
 */
#define SMP_LISTN_HILICHANGED			0x0001

/**
 * \brief Sent when KEY SELECT up. \�ķ��ͼ�ѡ���ʱ����
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_SELECTED)
 *		{
 *			int index = (int)lParam;
 * 			//�����֪ͨ��Ϣ
 *		}
 * \endcode
 *
 * \param index current high light index\����ָ��Ŀǰ�ĸ߹�ָ��
 */
#define SMP_LISTN_SELECTED			0x0002

/**
 * \brief Sent when MOUSE up.\����ʱ���
 *
 * \code
 *	case WM_COMMAND:
 *		WID id = LOWORD(wParam);
 *		WORD code = HIWORD(wParam);
 *
 *		if(id == "the list id" && code == SMP_LISTN_CLICKED)
 *		{
 *			int index = (int)lParam;
 * 			//handle the notify message/�����֪ͨ��Ϣ

 *		}
 * \endcode
 *
 * \param index current high light index * \����ָ��Ŀǰ�ĸ߹�ָ��
 */
#define SMP_LISTN_CLICKED				0x0003

/**
 * \brief Max supported collumns.  \�����֧��collumns
 */
#define SMP_LIST_MAX_STR_COL		2

/**
 * \����Ϣ
 */
typedef struct SMP_RowData
{	
	Uint16 margin;/* ��߾� */	
	Uint16 colWidth0;	/* ͼ��� */
	uint16 colHeight0;
	HBITMAP hBmp;/* ͼ���� */	
	struct{ /* ��Ϣ */	
		PCWSTR str;/* �ַ���*/		
		Uint16 width;/* ��*/
	}cols[SMP_LIST_MAX_STR_COL];	
} SMPROWDATA, *PSMPROWDATA;
/**
 * \brief The function provide to list to get total row count.\�Ĺ����ṩ���������������
 *
 * \return the total rows of the list \�����б��е�������
 */
typedef int (*SMP_List_GetTotal)(VOID);

/**
 * \brief The function provide to list to get row data. \�Ĺ����ṩ���������������
 *
 * \param index the row index\����ָ���������
 * \param[out] pRowData the row data\����[��] pRowData������
 */
typedef VOID (*SMP_List_GetRowData)(int index, PSMPROWDATA pRowData);

/**
 * \brief The draw row function.\�ĳ�ǩ�й���
 *
 * \param index the row index\�ĳ�ǩ�й���
 * \param r the r rectangle\����r��?����
 * \param pRowData the row data \����pRowData������
 * \param hilight the high light row index\���������߹�������
 * \param flush if flush to the screen \����ˢ�µ���Ļ�ϣ����ˢ��
 */
typedef VOID (*SMP_List_DrawRow)(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush);

/**
	���ڳ�Ա����

 * \brief Set user data provide callbacks. \�����û��������ṩ�˻ص�
 *
 * \param hList the list window handle \����hList�б��ھ��
 * \param fnGetTotal the function to get total row count \����fnGetTotal�ĺ�������ȡ������
 * \param fnGetRowData the function to get row data by index that from 0 
 \����fnGetRowData�������õ������������ݣ���0
 */
VOID SMP_List_SetDataProvider(HWND hList, SMP_List_GetTotal fnGetTotal, SMP_List_GetRowData fnGetRowData);

/**
 * \brief Set a user specific drawing function.\�������û��ض��Ļ�ͼ����
 *
 * \param hList the List window handle \����hList�б��ھ��
 * \param fnDrawRow the new draw row function \����fnDrawRow ROW�������»�
 */
VOID SMP_List_SetDrawRowCallback(HWND hList, SMP_List_DrawRow fnDrawRow);

/**
 * \brief The default draw row function.\��Ĭ���л��ƹ���
 *
 * \param index the index of the list item \�����������б��������
 * \param r the list item draw rect\ r�Ĳ����б�����ƾ���
 * \param pRowData the row data\����pRowData������
 * \param hilight current hilight list item index\����������ǰ�����б��������
 * \param flush if flush to screen\�����ϴ����ˢ����Ļ
 */
VOID SMP_List_DefaultDrawRowCallback(int index, PRECT r, PSMPROWDATA pRowData, int hilight, BOOL flush);

/**
 * \brief High light a row by index.\�����߹ⰴ������
 *
 * \param hList the List window\����hList�б���
 * \param index the row index\����ָ���������
 */
VOID SMP_List_HilightByIndex(HWND hList, int index);

/**
 * \brief Get current high light row index.\��ȡ��ǰ�ĸ߹�������
 *
 * \param hList the List window handle \����hList�б��ھ��
 * \return the row index\���ظ��е�����
 */
int SMP_List_GetHilightIndex(HWND hList);

/**
 * \brief Get the current page start item index and page size.
 * \��ȡ��ǰҳ������ָ����ҳ���С
 *
 * \param hList the list window handle\����hList�б��ھ��
 * \param[out] pagestart the current page start list item\����[��] pagestart��ǰҳ��ʼ�б���
 * \param[out] pagesize the list page size\����[��]ÿҳ��С���б�ҳ
 */
VOID SMP_List_GetPageInfo(HWND hList, int* pagestart, int* pagesize);

/**
 * \brief Page up the list.ҳ��һ����
 *
 * \param hList the list window handle����hList�б��ھ��
 */
VOID SMP_List_PageUp(HWND hList);

/**
 * \brief Page down the list. \��һҳ�嵥
 *
 * \param hList the list window handle\����hList�б��ھ��
 */
VOID SMP_List_PageDown(HWND hList);

/**
 * \brief Line up the list. \��������
 *
 * \param hList the list window handle\����hList�б��ھ��
 */
VOID SMP_List_LineUp(HWND hList);

/**
 * \brief Line down the list.\���б���������
 *
 * \param hList the list window handle\����hList�б��ھ��
 */
VOID SMP_List_LineDown(HWND hList);

/**
 * \brief Notify list that total list items changed.\֪ͨ�б����嵥��Ŀ���
 *
 * \param hList the list window handle\����hList�б��ھ��
 */
VOID SMP_List_Refresh(HWND hList);

/**
 * \brief Reset the list. \�����б�
 * 
 * \param hList the list window handle\����hList�б��ھ��
 */
VOID SMP_List_Reset(HWND hList);

/*
���ڳ���
 * \brief The simple list window procedure.\�ļ��б�Ĵ��ڹ���
 *
 * \param hList the window handle* \����hList�Ĵ��ھ��
 * \param Msg the window message * \������Ϣ�Ĵ�����Ϣ
 * \param wParam the first parameter \����wParam�ĵ�һ������
 * \param lParam the second parameter \����lParam�ĵڶ�������
 * \return the result of message process  \������Ϣ�Ĵ�����
 */
LRESULT SMP_List_WndProc(HWND hList, UINT Msg, WPARAM wParam, LPARAM lParam);


#endif /* _SMP_LIST_H */

