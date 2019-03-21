#ifndef _MOMO_H__
#define _MOMO_H__

#include "mrc_base.h"
#include "mrc_bmp.h"
#include "mrc_exb.h"
#include "mrc_image.h"
#include "mrc_base_i.h"
#include "mrc_skyfont.h"




#define iswhite(c) ((c) == ' ' || (c) == '\t')
#define isalpha(c) (((c)>='a'&&(c)<='z')||((c)>='A'&&(c)<='Z'))
#define isdigit(c) ((c)>='0'&&(c)<='9')
#define toupper(c) ((c)&=0xdf)//ת��Ϊ��д��ĸ
#define tolower(c) ((c)|=0x20)//ת��ΪСд��ĸ






extern char enable_smallfont;//�Ƿ�����С����
extern char FONT_W;
extern char FONT_H;
extern uint16 SCREEN_WIDTH;
extern uint16 SCREEN_HEIGHT;
extern int32 func_state;//��������״̬λ



//32λ��������״̬
#define _FUNC_SET_STYLE(state) \
	(func_state |= (state))

#define FUNC_CLR_STYLE(state) \
	(func_state &= ~(state))

#define _FUNC_IS_SET_ANY(state) \
	(func_state & (state))

#define FUNC_INITSUC 0x00000001
#define FUNC_EXIT    0x00000002
#define FUNC_CHARGEINIT       0x00000004
#define FUNC_STATE  0x00000008

#define FUNC_STATE3      0x00000000
#define FUNC_STATE4      0x00000000
#define FUNC_STATE5      0x00000000
#define FUNC_STATE6      0x00000000
#define FUNC_STATE7      0x00000000
#define FUNC_STATE8      0x00000000
#define FUNC_STATE9      0x00000000
#define FUNC_STATE10     0x00000000



#define MPRINTF(...) \
    do{\
    int32 h31415926len;\
    char msgBuffer[256];\
    int32 h31415926 = mrc_open("debug.txt", MR_FILE_CREATE|MR_FILE_WRONLY);\
    mrc_seek(h31415926, 0, MR_SEEK_END);\
    h31415926len = MR_SPRINTF(msgBuffer, __VA_ARGS__);\
    mrc_write(h31415926, msgBuffer, h31415926len);\
    mrc_write(h31415926, "\r\n", 2);\
    mrc_close(h31415926);\
    }while(0)


enum {
    CFG_SMALLFONT,
    CFG_FIRSTRUN,
    CFG_IMGDECODEMODE,//ͼƬת��ʱ�Ľ�����ѡ��
    CFG_CHARGE//�Ʒ�����
};
//��ȡ���ã��ɹ���������ֵ��ʧ�ܷ���FALSE
#define readCfg(item)    RdWrCfg(item,0,TRUE)

//д����,����ֵ������
#define writeCfg(item,data)   RdWrCfg(item,data,FALSE)

int32 RdWrCfg(uint32 item, int32 data, int32 isRead);





void iprint(int32 v,int x,int y);



//Ϊͨ��ȫ�ֱ�����ʼ��
int momo_init(void);

int momo_exit(void);


//˫��Բ�Ƿ���
void drawMyRectangle(int x,int y,int w,int h, uint32 color);


//�����ļ�
int CopyFile(const char* dst,const char* src);


//��ȡ��ǰϵͳĿ¼(��ʼ��ʱ����,֮ǰ��Ҫ���ļ�����,�ɹ�����·�����ȡ�ʧ�ܷ���MR_FAILED)
int32 GetCurSystemPath(char * out);

//��ȡĬ��ϵͳĿ¼
int32 GetDefSystemPath(char* out);

//���ļ�·������ļ�����:"c:/mythroad/a.txt"����a.txt,��Ҫfree
//·����'/'��'\'������
char* ExtractFileName(char* path);


//���ļ�·������ļ�·��,��ҪFREE,ĩβ��"\"
//·����'/'��'\'������
char *getPathPath(char* path);


//���ұ߿�ʼ��·���ָ���,��'/'��'\'������
char *getPathSeparator(char*path);

//�ַ����ӽ���
char *stred(char *str, int len);

//�����Ľ�����
void MidShadeLine(int x, int y, int w, uint32 pixelA, uint32 pixelB);

/*����һ��

����
f       �ļ����
out     ����ַ���
�ɹ�����0,ʧ�ܷ���-1
*/
int readline(int32 f, char out[256]);


/*������Ļ(��С����)*/
void TextScreen(const char *text, int8 is_unicode);

//ɾ��һ���ļ���(����ɾ��)
int32 removeDir(const char* dirName);

//����Ƿ��޸�
int32 Check(char *key);

//���ӿո�
char *AddWhite(char *str, int32 len);

/*
 �л�MRP����Ŀ¼
 strDiskΪĿ¼�ַ���,��"A:/","B:/"
*/
int32 SwitchDisk(const char* strDisk);  

enum E_SHAD_MODE
{
	SHADE_UPDOWN,		//���ϵ���
	SHADE_LEFTRIGHT,	//������
	SHADE_DOWNUP,		//���µ���
	SHADE_RIGHTLEFT		//���ҵ���
};
/*
��һ���������
���룺
xy			xyλ��
wh			��ȸ߶�		
pixelA		��ʼ��ɫ
pixelB		������ɫ
*/
void DrawShadeRect(int16 x, int16 y, int16 w, int16 h, uint32 pixelA, uint32 pixelB, enum E_SHAD_MODE mode);

//ͬ��
void ShadeRect(int x, int y, int w, int h, int AR, int AG, int AB, int BR, int BG, int BB, enum E_SHAD_MODE mode);

/*��һ��������Ӱ*/
void DrawHatching(int x, int y, int w, int h);

/**/
/*��һ���߿�*/
void DrawRectangle(int x, int y, int w, int h, uint32 pixel);

/*��һ������һ����ȵ��߿�*/
void DrawRectangle2(int x, int y, int w, int h, int line, uint32 pixel);

//ȷ�������Ƿ����㹻�Ŀռ�
int32 getDiskSpace(void);


#define PIXEL888R(pixel)      (uint8)(((pixel) >> 16) & 0xff)
#define PIXEL888G(pixel)      (uint8)(((pixel) >> 8) & 0xff)
#define PIXEL888B(pixel)      (uint8)((pixel) & 0xff)

/* ��һ����ɫ�� */
#define FillBox(x, y, w, h, pixel) \
	mrc_drawRect((int16)(x), (int16)(y), (int16)(w), (int16)(h), PIXEL888R((pixel)), PIXEL888G((pixel)), PIXEL888B((pixel)))

/* ��һ��ˮƽ�� */
#define DrawHLine(x, y, w, pixel) \
	mrc_drawLine((int16)(x), (int16)(y), (int16)((x)+(w)-1), (int16)(y), PIXEL888R(pixel), PIXEL888G(pixel), PIXEL888B(pixel))

void DrawHatchingText(const char *Text, int16 x, int16 y, int is_unicode);

typedef struct COPYMEN{
	int (*arry)[32][32];
	int x;
	int y;
	int w;
	int h;
}COPYMENDATA;

//�����ڴ��
void copymem(COPYMENDATA *dest, COPYMENDATA *src);

void BMPScreen(const char *name, int16 w, int16 h,uint16 Mode, int32 BackColor);

//��ȡ����ʱ��
void GetLocalTimer(char output[6]);

/* ��Ļ�м���ʾ���� (��С����)*/
void TextMiddle(const char *text, int8 is_unicode, int8 show);

//��ȡ�ֻ�������Ŀ��
// ����:
// MR_SUCCESS  �����ɹ�
// MR_FAILED      ����ʧ��
int GetMediumFontWH(int32 *w,int32 *h, uint16 font);

//�����ַ������(��С����)
int32 strW(char* str, int is_uincode, uint16 font);

//�����ַ����߶�(��С����)
int32 strH(char* str, int is_uincode, uint16 font);

/*
���ļ�·����ȡ��չ��,��Ҫfree,"bbb.c"����"C"
ע��:path�����ݻ�ı�!���ص���չ����ת��Ϊ��д
�ɹ������ļ���չ��ָ��,ʧ�ܷ��ؿ�
*/
char* GetFileExt(const char* path);


//����һ�����ȵ�GB�ַ����Ƿ�ȫ�����ذ�ȫ�ĳ���
int32 TestStr(char *str, int32 len);

//����һ��·��,�ɹ�����0ʧ�ܷ���-1
int32 MakePath(char *path);

//��������
void ShadeLine(int x, int y, int w, uint32 pixelA, uint32 pixelB);

//����12��������(ֻunicode)
int32 skyfontWidthHeight(char *szText, int32 *width, int32 *height);

int32 skyfontdraw(char* szText, int16 x, int16 y, mr_screenRectSt rect, mr_colourSt colorst, uint16 flag);


//ɾ���ַ����������ߵĿո�
char *strtrim(char *s);


/*
��ȡMRP�ڲ��ļ���Ϣ
MRPhandle[i]        MRP�ļ����
filename[i]         Ҫ��ȡ��Ϣ���ļ���
filepos[o]          �ļ���MRP�е�λ��
filelen[o]          �ļ���MRP�еĳ���
iszip[o]            �ļ���MRP�з�����ѹ����ʽ���
�ɹ�����0��ʧ�ܷ���-1
*/
int32 GetMRPFileInfo(int32 MRPhandle, char *filename, int32 *filepos, int32 *filelen, int32 *iszip);

//����GB�ַ������ַ���
int32 StrNum(char *str);

//�����ļ�����ͼƬ,ͼƬ��ʽ����ΪBMP24,JPG,GIF��ƽ̨֧�ֵĸ�ʽ
//�ɹ�����MR_SUCCESS,ʧ�ܷ���MR_FAILED��δʵ�ַ���MR_IGNORE.
int DrawIMG(char* filename, int x,int y);


#endif