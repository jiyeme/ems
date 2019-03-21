#ifndef _MAINWND_H
#define _MAINWND_H

#include "window.h"



/*
ͼ����ɫ�ö�����λ��������1������ɫ��0������ɫ��

ANM��ɫ������ʽ����16*16����Ϊ������
0401 0004 0000 //�ļ�ͷ�����ܻ�������������Ϣ��
1000 //ͼ���
1000 //ͼ���
8000 //ͼ�������ܴ�С
������ͼ������
12-44 //��һ֡ͼ��
44-76 //�ڶ�֡ͼ��
76-108 //����֡ͼ��
108-140 //����֡ͼ��
=============================================
EMS��ɫͼƬ��ʽ����32*32����Ϊ������
0201 00 //�ļ�ͷ
2000 //ͼ���
2000 //ͼ���
8000 //ͼ�������ܴ�С
������ͼ������
9-137 //ͼ������
*/

extern const char TEMPEMS[];
extern const char IMAGES[];
extern const char ANMDIR[];
extern const char EMSDIR[];


#define DrawEmsEditPointEx(wx, wy, color) \
    do{\
        drawColor=color?0:255;\
        mrc_drawRect((int16)(9+(wx)*g_wgsize),(int16)(9+(wy)*g_wgsize),fangkuaiaa,fangkuaiaa,drawColor,drawColor,drawColor);\
    }while(0)

#define DrawEmsEditPoint(x,y)	DrawEmsEditPointEx((x),(y),ems_data[(x)][(y)]);




int32 SaveAnm(char *FiName);
int32 SaveEms(char *FiName);





extern uint8 ems_data[32][32]; 






LRESULT MAINWND_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

#endif
