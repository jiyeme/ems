#ifndef __GET_ADV_H__
#define __GET_ADV_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


#include "mrc_base.h"



typedef struct advInfo
{
	int32 price;            //��浥�ۣ�����ҵ�λ���֣�
	int32 quota;            //��Ͷ��������ҵ�λ��Ԫ��
	int32 remain;           //����������ҵ�λ���֣�
	int32 appid;
	char shortname[8];
	char app_name[50];		//Ӧ������
	char app_summary[250];	//Ӧ�ü��
}st_advInfo;

typedef struct advData
{
	int32 realnum;        //��ʵ�õ�������
	st_advInfo* infoList;
}st_advData;

/*
��ȡ�����Ϣ�Ļص�����
code = 0ʱ��Ч
���ݴ��ڽṹ��st_advData��������Ҫ�ͷ�
*/
typedef void (*getAdv_cb)(int32 code,st_advData* pdata);

/*
  ��������getAdv
  �������:
  num:��Ҫ��ȡ�Ĺ���������100��Ĭ�Ϸ���10
  getAdv_cb:��ȡ�����ʱ�Ļص�
*/
int32 getAdv(int32 num,getAdv_cb CB);

/*
  ��������getadv_uninit
  �������:
  �����Ϸ�˳�ʱ��δ�յ��ص�����Ҫ���ô˺�������һ������
*/
void getadv_uninit(void);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GET_ADV_H__ */