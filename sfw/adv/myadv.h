#ifndef _MYADV_H_
#define _MYADV_H_

#ifdef __cplusplus
extern "C" {
#endif 

#define MAX_PATH_LEN 64

typedef struct tag_myData
{
    char* text;    //�������(����)
    char  path[MAX_PATH_LEN]; //�����Ƽ�.MRP
    int32 appid;
    int32 isExist; //�Ƿ����������Ӧ��
}st_myData;


//��������ʱ�������ز��,������ٺ��������ص����ػ���Ļ(����)
typedef void (*advPaintBack)(void);

//�����ͣ
void advPause(void);

//���ָ�
void advResume(void);

//��һ�����
void advNext(void);

//������
void advDl(void);

//ʹ�ù��(��ʼ��ʱ����)
void advInit(advPaintBack callback);

//������ʾ����
void advSetRect(int16 x, int16 y, int16 w, int16 h);

//ֹͣ���(����ʱ����)
void advDelete(void);



#ifdef __cplusplus
}
#endif 

#endif
