#include "i18n.h"
#include "mainwnd.h"
#include "momo.h"
#include "smp.h"
#include "smp_menu.h"
#include "smp_toolbar.h"
#include "smp_textinfo.h"
#include "smp_menuwnd.h"
#include "smp_edit.h"
#include "smp_button.h"
#include "smp_msgbox.h"
#include "smp_flashbox.h"
#include "bmp.h"
#include "smp_label.h"
#include "smp_progbar.h"
#include "smp_combobox.h"
#include "smp_list.h"
#include "topwnd.h"
#include "string.h"
#include "smp_titlebar.h"
#include "opendlg.h"
#include "preview.h"
#include "smp_inputbox.h"
// #include "smp_advbar.h"
#include "help.h"
#include "convert.h"

static const DWORD miOptions[] = 
{
    STR_DRAWPIC,
    STR_TOOL,
    STR_FILE,
    STR_OTHER,
    STR_EXIT
};
static const DWORD otherOptions[] =
{
    STR_SMALLFONT,
    STR_ABOUT,
    STR_HELP
};
static const DWORD drawpicOptions[] =
{
    STR_DRAWLINE,
    STR_DRAWCIR,
    STR_DRAWRECT1,
    STR_DRAWRECT2
};
static const DWORD toolOptions[] =
{
//     STR_COPYBLOK,
//     STR_PASTEBLOK,
//     STR_JX,                   //����
    STR_UNDO,//����
    STR_ADDTXT,
    STR_OPENTEMPLET,
    STR_WHITE,
    STR_BLACK,
    STR_REVERSE
};
static const DWORD fileOptions[] =
{
    STR_OPEN,
    STR_SAVE,
    STR_SAVEEMS,
    STR_SAVEANM
};

//�Ӵ���ID
enum
{
	MAINWND_EDIT = 1,
	MAINWND_TOOLBAR,
	MAINWND_OPTMENU,
	MAINWND_MSGBOX_EXIT,
    MAINWND_INPUT_SAVEMS,
    MAINWND_INPUT_SAVANM,
    MAINWND_INPUT_ADDTXT,
    MAINWND_MSG_TOEMSDIR,
    MAINWND_MSG_ADDTXT,
    MAINWND_MSG_TOANMDIR,
    MAINWND_MSG_SLTCOLOR,//ѡ����ɫ
    MAINWND_MSG_SLTCOLORTXT

};
/////////////////////////////////////////////////
#define HUATU          0x0001 //�������ò���ʹ�����ִ��
#define HUAYUAN        0x0002 //��Բ
#define HUAX           0x0004 //����
#define HUAJUXING      0x0008 //������
#define HUAXIANKUANG   0x0010  //���߿�
#define HUATUPOINTOK   0x0020 //��ͼ�Ƿ�ѡ���˵�һ����
#define HUATEXT        0x0040

#define _SET_STATE(v) \
    (state |= (v))

#define _CLR_STATE(v) \
    (state &= ~(v))

#define _IS_SET_STATE(v) \
    (state & (v))

static int16 state;
//////////////////////////////////////////////////////
const char TEMPEMS[]="����ͼƬ/EMS/temp.ems";
const char IMAGES[] = "����ͼƬ";
const char ANMDIR[]="����ͼƬ/ANM/";
const char EMSDIR[]="����ͼƬ/EMS/";
#define BACKCLOR             0,77,153
#define WAMGGECOLOR          255,128,0

uint8 ems_data[32][32];//ͼ������
uint8 ems_snap[32][32];//����ͼ������
uint8 ems_temp[32][32];//ͼ������

static int8 wzxd,wzyd;
static int8 wz_x,wz_y,scwzx,scwzy,lx;//��ǰ����XY���ϴ�����xy���Ƿ���������
static int8 g_wangge;                                //�Ƿ���ʾ����
static int8 g_wgsize,fangkuaiaa;                                //������С��5����7
static int8 l_x,l_y,g_color,g_selectcolor,drawColor;                          //����ʱ���ڼ�¼���,������ɫ1�Ǻ�ɫ
static int8 lastx,lasty; //��Բ���ߵȹ��ܵĵ�һ��ȡ��(�������꣬����Ļ���꣩
static int8 gIsCharToEms,CEmsW,CEmsH;//�Ƿ�������תEMS״̬������EMS���
static int16 g_wgw; //������
static HWND hToolbar;
static mr_screenRectSt gColor,gLx,gWangge,gYulan;     //��������
char path[256];
char tempName[64];



/////////////////////////////////////////////////////////

static int OpenSmsImg(char *FiName, uint8 out_put_data[32][32])
{
    int32 handle;
    uint32 w,h,b=0;
    uint8 n,i,a=0,p,aa=0,bb=0,cc,dd;
    uint8 data[140];

    h=mrc_getLen(FiName);
    handle = mrc_open(FiName,MR_FILE_RDONLY);
    if(!handle) return -1;
    if(137==h)//��EMS��ʽ
    {
        mrc_read(handle,data,137);
        i=9;
        for(h=0;h<=31;h++)//����ת���ɶ�����
        {
            for(a=1;a<=4;a++)
            {
                w=a*8-1;
                n=data[i++];	
                for(b=1;b<=8;b++)
                {
                    if(n%2)
                        out_put_data[w][h] = 1;
                    n=n>>1;
                    w--;
                }
            }
        }

    }
    else if(140==h)//��ANM��ʽ
    {
        mrc_read(handle,data,140);
        i=12;

        for(p=0;p<4;p++)
        {
            switch(p)
            {
            case 0:
                aa=0,a=15;
                bb=1,b=2;
                break;
            case 1:
                aa=0,a=15;
                bb=3,b=4;
                break;
            case 2:
                aa=16,a=31;
                bb=1,b=2;
                break;
            case 3:
                aa=16,a=31;
                bb=3,b=4;
            }
            for(h=aa;h<=a;h++)//����ת���ɶ�����
            {
                for(cc=bb;cc<=b;cc++)
                {
                    w=cc*8-1;
                    n=data[i++];	
                    for(dd=0;dd<=7;dd++)
                    {
                        if(n%2)
                            out_put_data[w][h] = 1;
                        n=n>>1;
                        w--;
                    }
                }
            }
        }
    }
    else
    {
        mrc_close(handle);
        return -1;
    }
    mrc_close(handle);
    return 0;
}

static VOID ShowOptMenu(HWND hWnd)
{
    SMP_Menu_ClearMenuItems();
    SMP_Menu_SetMenuItem2(0, miOptions, TABLESIZE(miOptions));
    SMP_Menu_SetMenuItem2(TABLESIZE(miOptions), drawpicOptions, TABLESIZE(drawpicOptions));
    SMP_Menu_SetMenuItem2(TABLESIZE(miOptions)+TABLESIZE(drawpicOptions), toolOptions, TABLESIZE(toolOptions));
    SMP_Menu_SetMenuItem2(TABLESIZE(miOptions)+TABLESIZE(drawpicOptions)+TABLESIZE(toolOptions), fileOptions, TABLESIZE(fileOptions));
    SMP_Menu_SetMenuItem2(TABLESIZE(miOptions)+TABLESIZE(drawpicOptions)+TABLESIZE(toolOptions)+TABLESIZE(fileOptions), otherOptions, TABLESIZE(otherOptions));

    if(readCfg(CFG_SMALLFONT))//С����
        SMP_Menu_CheckMenuItem(TABLESIZE(miOptions)+TABLESIZE(drawpicOptions)+TABLESIZE(toolOptions)+TABLESIZE(fileOptions),TRUE);

    SMP_Menu_SetSubMenu(0,TABLESIZE(miOptions));
    SMP_Menu_SetSubMenu(1,TABLESIZE(miOptions)+TABLESIZE(drawpicOptions));
    SMP_Menu_SetSubMenu(2,TABLESIZE(miOptions)+TABLESIZE(drawpicOptions)+TABLESIZE(toolOptions));
    SMP_Menu_SetSubMenu(3,TABLESIZE(miOptions)+TABLESIZE(drawpicOptions)+TABLESIZE(toolOptions)+TABLESIZE(fileOptions));

    SMP_Menu_Popup(MAINWND_OPTMENU, SMP_MENUS_BOTTOMLEFT, hWnd, 0, _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, NULL);
}



static void chonghua(int32 asdf)
{
    static int16 wzx,wzy,a,b,c,d;

    //���ֲ�����
    DrawEmsEditPoint(wz_x,wz_y);//��ǰλ��
    //�ϴ�λ�ø���ѡ���
    if(scwzy) DrawEmsEditPoint(scwzx,scwzy-1);//��
    if(scwzy != 31) DrawEmsEditPoint(scwzx,scwzy+1);//��
    if(scwzx) DrawEmsEditPoint(scwzx-1,scwzy);//��
    if(scwzx != 31) DrawEmsEditPoint(scwzx+1,scwzy);//��
    if (gIsCharToEms)
    {
        uint8 x,y,i,j,c,w,h;

        for(y=0; y<=31; y++)//�����飨ȫ����
        {
            for(x=0; x<=31; x++)
            {
                drawColor=ems_data[x][y]?0:255;
                mrc_drawRect((int16)(9+x*g_wgsize),(int16)(9+y*g_wgsize),fangkuaiaa,fangkuaiaa,drawColor,drawColor,drawColor);
            }
        }

        c = g_selectcolor ? 0 : 255;
        h=(wz_y+CEmsH>31) ? CEmsH-(wz_y+CEmsH-31) : CEmsH;//��֤ѡ��������λ��ʱ������Խ��
        w=(wz_x+CEmsW>31) ? CEmsW-(wz_x+CEmsW-31) : CEmsW;
        for(y=wz_y,i=0; i<=h; y++,i++)//�����飨ת��������EMS��
            for(x=wz_x,j=0; j<=w; x++,j++)
                if(convertEms[j][i]==g_selectcolor)
                    mrc_drawRect((int16)(9+x*g_wgsize),(int16)(9+y*g_wgsize),fangkuaiaa,fangkuaiaa,c,c,c);
    }

    wzx = 8 + scwzx * g_wgsize;
    wzy = 8 + scwzy * g_wgsize;
    a = wzx + g_wgsize;
    b = wzy + g_wgsize;
    c = wzy-1;
    d = wzx-1;

    mrc_drawLine(wzx, c  , wzx ,b+1, BACKCLOR);//�����
    mrc_drawLine(a  , c  , a   ,b+1, BACKCLOR);//���ұ�
    mrc_drawLine(d  , wzy, a+1 ,wzy, BACKCLOR);//���ϱ�
    mrc_drawLine(d  , b  , a+1 ,b  , BACKCLOR);//���±�

    if(g_wangge)
    {	//������
        if(!(scwzx % 8))
            mrc_drawLine(wzx, c  , wzx ,b+1, WAMGGECOLOR);//�����
        if(!((scwzx+1) %8))
            mrc_drawLine(a  , c  , a   ,b+1, WAMGGECOLOR);//���ұ�
        if(!(scwzy % 8))
            mrc_drawLine(d  , wzy, a+1 ,wzy, WAMGGECOLOR);//���ϱ�
        if(!((scwzy+1) %8))
            mrc_drawLine(d  , b  , a+1 ,b  , WAMGGECOLOR);//���±�
    }

    if(!scwzx || !scwzy || scwzx == 31 || scwzy == 31)//��ѡ����ڱ߽���ʱ,��Ҫ�����
        GAL_Rectangle(7,7,g_wgw+3,g_wgw+3,0x004D99);


    //��ѡ���
    wzx = 8 + wz_x * g_wgsize;
    wzy = 8 + wz_y * g_wgsize;
    a = wzx + g_wgsize;
    b = wzy + g_wgsize;
    c = g_wgsize+1;

    mrc_drawRect(wzx  , wzy-1, c, 2, 240,0,0);//���ϱ�
    mrc_drawRect(wzx  , b    , c, 2, 240,0,0);//���±�
    mrc_drawRect(wzx-1, wzy  , 2, c, 240,0,0);//�����
    mrc_drawRect(a    , wzy  , 2, c, 240,0,0);//���ұ�

    if(asdf) GAL_Flush();
}
static void The2To10(int16 kx, int16 iy, int16 b, int16 *Data)
{
    if(ems_data[kx][iy])
    {
        switch(b)
        {
        case 0:
            *Data += 1;
            break;
        case 1:
            *Data += 2;
            break;
        case 2:
            *Data += 4;
            break;
        case 3:
            *Data += 8;
            break;
        case 4:
            *Data += 16;
            break;
        case 5:
            *Data += 32;
            break;
        case 6:
            *Data += 64;
            break;
        case 7:
            *Data += 128;
        }
    }
}
int32 SaveAnm(char *FiName)//����ANM
{ 
    int16 Data;
    int32 handle;
    int16 iy,kx,sa,sb;
    int16 saa,sbb,sx,sy,sp;

    saa=sbb=sa=sb=0;
    mrc_remove(FiName);
    handle = mrc_open(FiName,MR_FILE_WRONLY | MR_FILE_CREATE);
    if(!handle) 
        return MR_FAILED;
    mrc_write(handle,"\x04\x01\x00\x04\x00\x00\x10\x00\x10\x00\x80\x00",12);//д�ļ�ͷ(12�ֽ�)

    for(sp=0;sp<4;sp++)
    {
        switch(sp)
        {
        case 0:
            saa=0,sa=15;
            sbb=0,sb=1;
            break;
        case 1:
            saa=0,sa=15;
            sbb=2,sb=3;
            break;
        case 2:
            saa=16,sa=31;
            sbb=0,sb=1;
            break;
        case 3:
            saa=16,sa=31;
            sbb=2,sb=3;
        }
        for(iy=saa;iy<=sa;iy++)//���濪ʼдͼ������
        {
            for(sx=sbb;sx<=sb;sx++)
            {
                kx=(sx+1)*8-1;
                Data=0;
                for(sy=0 ; sy<=7 ; sy++,kx--)//��תʮ
                {
                    The2To10(kx,iy,sy,&Data);
                }
                mrc_write(handle,&Data,1);
            }
        }
    }
    mrc_close(handle);
    return MR_SUCCESS;
}

int32 SaveEms(char *FiName)//����EMS
{ 
    int16 Data;
    int32 handle;
    int16 iy,kx,a,b;

    mrc_remove(FiName);
    handle = mrc_open(FiName,MR_FILE_WRONLY | MR_FILE_CREATE);
    if(!handle) 
        return MR_FAILED;   
    mrc_write(handle,"\x02\x01\x00\x20\x00\x20\x00\x80\x00",9);//д�ļ�ͷ(9�ֽ�)

    for(iy=0;iy<=31;iy++)//���濪ʼдͼ������
    {
        for(a=kx=0;a<=3;a++)
        {	
            kx=(a+1)*8-1;
            Data=0;
            for(b=0 ; b<=7 ; b++,kx--)//��תʮ
            {
                The2To10(kx,iy,b,&Data);
            }
            mrc_write(handle,&Data,1);
        }
    }
    mrc_close(handle);
    return MR_SUCCESS;
}


int8 IsPointInObject(int32 px, int32 py, mr_screenRectSt *Rect) //�ж��Ƿ�Ϊָ������
{ 
    if (px < Rect->x) return FALSE; 
    if (px > Rect->x + Rect->w) return FALSE; 
    if (py < Rect->y) return FALSE; 
    if (py > Rect->y + Rect->h) return FALSE; 
    return TRUE; 
} 
static void EmsDrawLine(int16 x1,int16 y1,int16 x2,int16 y2)
{    
    int32 x, y, dx, dy, s1, s2, p, temp, interchange, i;
    x = x1;
    y = y1;
    dx = x2 > x1 ? (x2 - x1) : (x1 - x2);
    dy = y2 > y1 ? (y2 - y1) : (y1 - y2);

    s1 = x2>x1 ? 1 : -1;
    s2 = y2>y1 ? 1 : -1;

    if(dy>dx)
    {
        temp=dx;
        dx=dy;
        dy=temp;
        interchange=1;
    }
    else interchange=0;

    p=(dy<<1)-dx;
    for(i=0;i<=dx;i++)
    {
        ems_data[x][y]=g_selectcolor;
        if(p>=0)
        {
            if(!interchange)
                y = y + s2;
            else
                x = x + s1;
            p = p-(dx<<1);
        }
        if(!interchange)
            x = x + s1;
        else
            y = y + s2;
        p = p+(dy<<1);
    }
}
static void EMSPOINT(int x,int y)
{
    if(x<0 || x>31 || y<0 || y>31) return;//����
    ems_data[x][y]=g_selectcolor;
}
static void EMSCirPoint(int x0,int y0,int x,int y)
{   
    EMSPOINT(x0+x,y0+y);
    EMSPOINT(x0+y,y0+x);
    EMSPOINT(x0-y,y0+x);
    EMSPOINT(x0-x,y0+y);
    EMSPOINT(x0+y,y0-x);
    EMSPOINT(x0+x,y0-y);
    EMSPOINT(x0-x,y0-y);
    EMSPOINT(x0-y,y0-x);
}
static void EmsDrawCircle(int x0,int y0,int r0)
{   
    int d,x,y;

    d = 1-r0;
    x = 0;
    y = r0;
    EMSCirPoint(x0,y0,x,y);
    while(y>x)
    {
        if(d<=0)
        {d=d+2*x+3;}
        else 
        {d=d+2*(x-y);y=y-1;}
        x=x+1;
        EMSCirPoint(x0,y0,x,y);
    }
}
static void EmsDrawRect(int x1, int y1, int x2, int y2, BOOL isfull)
{
    int i,w,h;

    w = x1>x2 ? x1-x2 : x2-x1;
    h = y1>y2 ? y1-y2 : y2-y1;
    x1 = x1<x2 ? x1 : x2;
    y1 = y1<y2 ? y1 : y2;
    x2 = x1+w;
    y2 = y1+h;
    if(isfull)
    {
        for(i=0; i <= h; i++)
            EmsDrawLine(x1, y1+i, x2, y1+i);
    }
    else
    {
        EmsDrawLine(x1, y1, x2, y1);
        EmsDrawLine(x1, y2, x2, y2);
        EmsDrawLine(x1, y1, x1, y2);
        EmsDrawLine(x2, y1, x2, y2);
    }
}

static void huaxian(int16 x2, int16 y2)
{
    int16 x, y, dx, dy, s1, s2, p, interchange, i;

    x = (int16)l_x;
    y = (int16)l_y;
    dx = x2 > l_x ? (x2 - l_x) : (l_x - x2);
    dy = y2 > l_y ? (y2 - l_y) : (l_y - y2);

    s1 = x2>l_x ? 1 : -1;
    s2 = y2>l_y ? 1 : -1;

    if(dy>dx)
    {
        dx^=dy;
        dy^=dx;
        dx^=dy;
        interchange=1;
    }
    else interchange=0;

    p=(dy<<1)-dx;
    for(i=0;i<=dx;i++)
    {
        ems_data[x][y] = (uint8)g_color;
        DrawEmsEditPointEx(x,y,g_color);
        if(p>=0)
        {
            if(!interchange)
                y += s2;
            else
                x += s1;
            p = p-(dx<<1);
        }
        if(!interchange)
            x += s1;
        else
            y += s2;
        p = p+(dy<<1);
    }
}
static void drawState(int32 asdf)
{	
    uint16 x,y;

    x = (SCREEN_WIDTH>>1)-88;
    y = 13+g_wgw;
    mrc_bitmapShow(3,x,y,BM_TRANSPARENT,0,0,176,20);

    if(g_color) mrc_drawRect(x+32, y+6, 12, 9,0,0,0);
    else mrc_drawRect(x+32, y+6, 12, 9,255,255,255);

    if(lx) mrc_drawRect(x+81, y+9, 4, 3,0x0033,0x00cc,0x0033);
    else mrc_drawRect(x+81, y+9, 4, 3,255,255,255);

    if(g_wangge) mrc_drawRect(x+125, y+9, 4, 3,0x0033,0x00cc,0x0033);
    else mrc_drawRect(x+125, y+9, 4, 3,255,255,255);
    if(asdf) GAL_Flush();
}

static void MainShow(int8 isref)//�����ڳ�ʼ��
{
    int16 x,y;

    mrc_drawRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT-SMP_ITEM_HEIGHT,BACKCLOR);
    y=scwzx=scwzy=0;
    for(; y<=31; y++)//�����飨ȫ����
        for(x=0;x<=31;x++)
            DrawEmsEditPoint(x,y);
    if(g_wangge) 
    {
        x^=x;
        y = 8;
        while(x<=g_wgw)//����
        {
            if(!(x%8))
            {
                mrc_drawLine(y,8,y,8+g_wgw,WAMGGECOLOR);//������
                mrc_drawLine(8,y,8+g_wgw,y,WAMGGECOLOR);//������
            }
            y+=g_wgsize,x=y-8;
        }
    }
    drawState(0);
    chonghua(isref);
}
static void CharToEmsOk(void)
{
    uint8 x,y,i,j,w,h;

    h=(wz_y+CEmsH>31) ? CEmsH-(wz_y+CEmsH-31) : CEmsH;//��֤ѡ��������λ��ʱ������Խ��
    w=(wz_x+CEmsW>31) ? CEmsW-(wz_x+CEmsW-31) : CEmsW;
    for(y=wz_y,i=0; i<=h; y++,i++)//�����飨ת��������EMS��
        for(x=wz_x,j=0; j<=w; x++,j++)
            if(convertEms[j][i]==g_selectcolor)
                ems_data[x][y]=convertEms[j][i];

}
static void CharToEms(int smallfont, PCWSTR str, HWND hWnd)
{
    int32 x,y,minx,miny,maxx=0,maxy=0;
    uint16 *scrbuf;

    mrc_clearScreen(0,0,0);
    if (smallfont)
    {
        mr_screenRectSt rect={0,};
        mr_colourSt colorst={255,255,255};

        rect.w=SCREEN_WIDTH;
        rect.h=SCREEN_HEIGHT;
        skyfontdraw((char*)str,0,0,rect,colorst,1);
        CEmsW=FONT_W; CEmsH=FONT_H;
    } 
    else
    {
        uint16 font=(uint16)SGL_GetSystemFont();
        mrc_drawText((char*)str,0,0,255,255,255,1,font);
        GetMediumFontWH(&x,&y,font);
        CEmsW=(int8)x;
        CEmsH=(int8)y;
    }                               
    scrbuf=w_getScreenBuffer();
    minx=CEmsW;
    miny=CEmsH;
    for(y=0; y<=CEmsH; y++)//ɾ�������ܱߵĿհ�
    {
        for(x=0;x<=CEmsW;x++)
        {
            if((uint16)*(scrbuf+x+y*SCREEN_WIDTH))//�����Ļָ�������ɫ
            {
                if(x<minx) minx=x;//��СX
                if(y<miny) miny=y;
                if(x>maxx) maxx=x;//���x
                if(y>maxy) maxy=y;
            }
        }
    }
    CEmsH=(int8)(maxy-miny);
    CEmsW=(int8)(maxx-minx);
    if(CEmsW<0 || CEmsH<0) return;
    for(y=0; y<=CEmsH; y++)//ת��ΪEMS
    {
        for(x=0; x<=CEmsW; x++)
        {
            if((uint16)*(scrbuf+minx+x+(miny+y)*SCREEN_WIDTH))//�����Ļָ�������ɫ
                convertEms[x][y]=g_selectcolor;
            else
                convertEms[x][y]=!g_selectcolor;
        }
    }
    gIsCharToEms=TRUE;
    SMP_Toolbar_SetStrings(hToolbar, STR_OK, RESID_INVALID, STR_CANCEL, TRUE);
    SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_STR2), ID_OK,NULL); 
}
static void MenuEvent(HWND hWnd, WORD code)
{
    switch(code)
	{
    case STR_DRAWLINE://����
        _SET_STATE(HUAX);
    case STR_DRAWCIR://��Բ
        _SET_STATE(HUAYUAN);
    case STR_DRAWRECT1://������
        _SET_STATE(HUAJUXING);
    case STR_DRAWRECT2://���߿�
        _SET_STATE(HUAXIANKUANG);
        _SET_STATE(HUATU);
        lx=FALSE;//��Ϊ�٣���ֹ��������ʱ����
        mrc_memcpy(ems_snap,ems_data,1024);//�������
        SMP_Toolbar_SetStrings(hToolbar, STR_OK, RESID_INVALID, STR_CANCEL, 1);
        
        SMP_MsgBox(MAINWND_MSG_SLTCOLOR,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_DRAWISBLACK),ID_OK|ID_CANCEL|ID_YESNO,NULL);
        return;
    case STR_ADDTXT://�������
        mrc_memcpy(ems_snap,ems_data,1024);//�������
        SMP_MsgBox(MAINWND_MSG_SLTCOLORTXT,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_DRAWISBLACK),ID_OK|ID_CANCEL|ID_YESNO,NULL);
        return;
	case STR_EXIT: // �˳�
        SMP_MsgBox(MAINWND_MSGBOX_EXIT, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_EXITDIALOG), ID_OK|ID_CANCEL|ID_YESNO , NULL);
		return;
	case STR_OPEN://���ļ�
		ShowOpenDlg(STR_OPEN,TOPWND_MAINWND, DIALOG_SELECTFILE);
		return;
    case STR_OPENTEMPLET://��ģ��
        ShowOpenDlg(STR_OPENTEMPLET,TOPWND_MAINWND,DIALOG_SELECTFILE);
        return;
    case STR_ABOUT: //����
        {
            UnicodeSt uniText;
            char tmp[256];
            int mainUsed,mainLeft,mainUp;

            mainUp=mrc_getMemStatus(&mainUsed, &mainLeft, NULL, NULL, NULL, NULL);
            mrc_sprintf(tmp,"Ĭˮƿ�������\n�ڴ�:\n��ֵ%dB\nʹ��%dB\nʣ��%dB",mainUp,mainUsed,mainLeft);
            CharToUnicode(tmp, &uniText.pText, &uniText.len);
            SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_ABOUT),(PWSTR)uniText.pText, ID_OK,NULL);
            mrc_freeOrigin(uniText.pText, uniText.len);
        }
        return;
    case STR_SAVE://��ԭ·������
        {
            int ret;
            int len;

            len = mrc_strlen(path);
            if(!len)
            {
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL); 
                return;
            }
            if(path[len-1] == 's')
                ret = SaveEms(path);
            else
                ret = SaveAnm(path);
            if(!ret)
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVESUC), ID_OK,NULL);
            else
            {
                path[0]=0;
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL); 
            }
        }
        return;
    case STR_SAVEEMS://���EMS
        SMP_InputBox(MAINWND_INPUT_SAVEMS,hWnd,SGL_LoadString(STR_SAVEEMS),SGL_LoadString(STR_PIC1),128,ES_ALPHA, NULL);
        return;
    case STR_SAVEANM://���ANM
        SMP_InputBox(MAINWND_INPUT_SAVANM,hWnd,SGL_LoadString(STR_SAVEANM),SGL_LoadString(STR_PIC1),128,ES_ALPHA, NULL);
        return;
    case STR_SMALLFONT://С����
        {
            char data;

            data=(char)readCfg(CFG_SMALLFONT);
            data=!data;
            writeCfg(CFG_SMALLFONT,data);
            SMP_MsgBox(0,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SETSUC),ID_OK,NULL);
        }
        return;
    case STR_HELP: //����
        ShowHELP(TOPWND_MAINWND,STR_HELPCONTENT);
        return;
        /////////////////////////////////////////////////////��������Ҫȫ���ػ���////////////////////////
    case STR_UNDO://����
        mrc_memcpy(ems_temp,ems_data,1024);
        mrc_memcpy(ems_data,ems_snap,1024);
        mrc_memcpy(ems_snap,ems_temp,1024);
        break;
    case STR_WHITE: //ȫ��
        mrc_memset(ems_data,0,sizeof(ems_data));
        break;
    case STR_BLACK://ȫ��
        mrc_memset(ems_data,1,sizeof(ems_data));
        break;
    case STR_REVERSE://ȫ�෴
        {
            int8 i,k;

            for(i=0;i<=31;i++)
                for(k=0;k<=31;k++)
                    ems_data[i][k] = !ems_data[i][k];
            break;
        }
    }
    MainShow(TRUE);//ȫ���ػ�
}
static void ShowEvent(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if(!_FUNC_IS_SET_ANY(FUNC_INITSUC) && mrc_fileState((PSTR)TEMPEMS)==MR_IS_FILE)
    {
        OpenSmsImg((char *)TEMPEMS,ems_data);
        _SET_USERDATA(hWnd,0);
    }
    _FUNC_SET_STYLE(FUNC_INITSUC);
    if(_USERDATA(hWnd) == DIALOG_GET)
    {
        int len=strlen(SMP_DIALOG_PATH);

        if(SMP_DIALOG_ID == STR_OPEN || SMP_DIALOG_ID == STR_OPENTEMPLET)
        {
            if(SMP_DIALOG_ID == STR_OPEN)
                mrc_memset(ems_data,0,sizeof(ems_data));//���ڴ�EMS�ʹ�ģ������ͬ��
            if(SMP_DIALOG_PATH[len-1] == 's')
            {
                if(OpenSmsImg(SMP_DIALOG_PATH,ems_data))
                {
                    SMP_MsgBox(0,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_LOADFAI),ID_OK,NULL);
                    path[0]=0;//��ʧ��Ҫ���·��
                }
                else                
                    mrc_strcpy(path, SMP_DIALOG_PATH);
            }
            else
            {
                if(OpenSmsImg(SMP_DIALOG_PATH,ems_data))
                {
                    SMP_MsgBox(0,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_LOADFAI),ID_OK,NULL);
                    path[0]=0;
                }
                else
                    mrc_strcpy(path, SMP_DIALOG_PATH);
            }
        }

        switch(SMP_DIALOG_ID)
        {
        case STR_SAVTOEMSDIR://EMS��������Ĭ��·��
            mrc_strcpy(path, SMP_DIALOG_PATH);
            mrc_strcat(path,"/");
            mrc_strcat(path,tempName);
            mrc_strcat(path,".ems");
            if(!SaveEms(path))
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVESUC), ID_OK,NULL);
            else
            {
                path[0]=0;//����ʧ��
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL); 
            }
            break;

        case STR_SAVTOANMDIR:
            mrc_strcpy(path, SMP_DIALOG_PATH);
            mrc_strcat(path,"/");
            mrc_strcat(path,tempName);
            mrc_strcat(path,".anm");
            if(!SaveAnm(path))
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVESUC), ID_OK,NULL);
            else
            {
                path[0]=0;//����ʧ��
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL); 
            }
            break;
        }
        _SET_USERDATA(hWnd,0);
    }
}
static void command(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    WID id = LOWORD(wParam);
    WORD code = HIWORD(wParam);
    
    switch(id)
    {
    case MAINWND_OPTMENU:
        MenuEvent(hWnd, code);
        break;
    case MAINWND_MSGBOX_EXIT://�˳�
        switch(code)
        {
        case ID_OK:
            _FUNC_SET_STYLE(FUNC_EXIT);
            mrc_exit();
            return;
        }
        break;
    case MAINWND_INPUT_SAVEMS://���EMS
        if(code == INPUT_OK)
        {
            PSTR tt;

            tt = UnicodeToChar((PWSTR)lParam);
            mrc_strcpy(tempName,tt);
            SGL_FREE(tt);
            SMP_MsgBox(MAINWND_MSG_TOEMSDIR, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_SAVTOEMSDIR), ID_OK|ID_CANCEL|ID_YESNO , NULL);
        }
        break;

    case MAINWND_INPUT_ADDTXT://�������ʱ�����ַ�
        if(code == INPUT_OK)
        {
            _SET_STATE(HUATU|HUATEXT);
            if(enable_smallfont)//���Ѿ�����С����,��ʾ�Ƿ���Ҫʹ�ô�����
            {
                wstrcpy((PWSTR)tempName,(PCWSTR)lParam);
                SMP_MsgBox(MAINWND_MSG_ADDTXT, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_USEBIGFONT), ID_OK|ID_CANCEL|ID_YESNO , NULL);        
            }
            else
                CharToEms(FALSE, (PCWSTR)lParam,hWnd);
        }
        break;
    case MAINWND_MSG_SLTCOLOR:
        switch(code)
        {
        case ID_OK:
            g_selectcolor=1;
            break;
        case ID_CANCEL:
            g_selectcolor=0;
            break;
        }
        SMP_MsgBox(0,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_FIRSTPOINT),ID_OK,NULL);
        break;
    case MAINWND_MSG_SLTCOLORTXT:
        switch(code)
        {
        case ID_OK:
            g_selectcolor=1;
            break;
        case ID_CANCEL:
            g_selectcolor=0;
            break;
        }
        SMP_InputBox(MAINWND_INPUT_ADDTXT,hWnd,SGL_LoadString(STR_INPUTCHAR), NULL, 4, ES_ALPHA, NULL);
        break;
    case MAINWND_MSG_ADDTXT://�������
        switch(code)
        {
        case ID_OK://������
            CharToEms(FALSE, (PWSTR)tempName,hWnd);
            break;
        case ID_CANCEL:
            CharToEms(TRUE, (PWSTR)tempName,hWnd);
            break;
        }
        break;

    case MAINWND_MSG_TOEMSDIR:
        switch(code)
        {
        case ID_OK:
            mrc_strcpy(path,EMSDIR);
            mrc_strcat(path,tempName);
            mrc_strcat(path,".ems");
            if(!SaveEms(path))
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVESUC), ID_OK,NULL);
            else
            {
                path[0]=0;
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL); 
            }
            break;
        case ID_CANCEL:
            ShowOpenDlg(STR_SAVTOEMSDIR,TOPWND_MAINWND, DIALOG_SELECTDIR);
            break;
        }
        break;

    case MAINWND_INPUT_SAVANM://���ANM
        if(code == INPUT_OK)
        {
            PSTR tt;

            tt = UnicodeToChar((PWSTR)lParam);
            mrc_strcpy(tempName,tt);
            SGL_FREE(tt);
            SMP_MsgBox(MAINWND_MSG_TOANMDIR, hWnd, NULL, SGL_LoadString(STR_MSG), SGL_LoadString(STR_SAVTOANMDIR), ID_OK|ID_CANCEL|ID_YESNO , NULL);
        }
        break;

    case MAINWND_MSG_TOANMDIR:
        switch(code)
        {
        case ID_OK:
            mrc_strcpy(path,ANMDIR);
            mrc_strcat(path,tempName);
            mrc_strcat(path,".anm");
            if(!SaveAnm(path))
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVESUC), ID_OK,NULL);
            else
            {
                path[0]=0;
                SMP_MsgBox(0,hWnd, NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SAVEFAI), ID_OK,NULL); 
            }
            break;
        case ID_CANCEL:
            ShowOpenDlg(STR_SAVTOANMDIR,TOPWND_MAINWND, DIALOG_SELECTDIR);
            break;
        }
        break;

    }//swithc(id)
}

static void revColor(int rawpoint)
{
    uint16 x,y;

    if(rawpoint)//�ı����ɫ
    {
        g_color = ems_data[wz_x][wz_y] = !ems_data[wz_x][wz_y];
        DrawEmsEditPointEx(wz_x,wz_y,g_color);
    }
    //�ı��������ʾ��ɫ
    x = (SCREEN_WIDTH>>1)-88+32;
    y = 19+g_wgw;
    if(g_color) mrc_drawRect(x, y, 12, 9,0,0,0);
    else mrc_drawRect(x, y, 12, 9,255,255,255);
    GAL_Flush();
}
LRESULT MAINWND_WndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    scwzx=wz_x , scwzy=wz_y;
	switch(Msg)
	{
	case WM_CREATE:
	{
        uint16 x,y;

        hToolbar = SGL_CreateWindow(SMP_Toolbar_WndProc, 0,
            _HEIGHT(hWnd) - SMP_ITEM_HEIGHT, 
            _WIDTH(hWnd), SMP_ITEM_HEIGHT, 
            MAINWND_TOOLBAR, 0, 0);
        SMP_Toolbar_SetStrings(hToolbar, STR_MENU, RESID_INVALID, STR_RETURN, 0);
        SGL_AddChildWindow(hWnd, hToolbar);


        g_color = ems_data[wz_x][wz_y] = 1;
        if(SCREEN_WIDTH == 176)
        {
            g_wgsize=5;
        }
        else
        {
            g_wgsize=7;
        }
        fangkuaiaa = g_wgsize-1;

        mrc_bitmapNew(3,176,20);
        mrc_bitmapLoad(3,"tool.bmp",0,0,176,20,176);

        g_wgw = g_wgsize*32;
        x = (SCREEN_WIDTH>>1)-88;
        y = 13+g_wgw;

        //״̬
        gColor.x = x+2;
        gLx.x = x+46;
        gWangge.x = x+90;
        gYulan.x = x+137;
        gYulan.y = gWangge.y = gLx.y = gColor.y = y+4;
        gYulan.h = gWangge.h = gLx.h = gColor.h = 12;
        gYulan.w = 35;
        gWangge.w = gLx.w = gColor.w = 43;

        mrc_mkDir(IMAGES);
        mrc_mkDir(ANMDIR);
        mrc_mkDir(EMSDIR);

		break;
	}
    case WM_DESTROY:
    {
        mrc_bitmapLoad(3,"*",0,0,176,20,176);
		return 0;
    }
    case WM_PAINT:
        MainShow(FALSE);
        break;
    case WM_SHOW:
        ShowEvent(hWnd,Msg,wParam,lParam);
        break;
    case WM_MOUSEMOVE:
    {
        int p1 = (int)wParam;
        int p2 = (int)lParam;

        //ѡ����ƶ�(����)
        wzxd=(int8)((p1-8)/g_wgsize) , wzyd=(int8)((p2-8)/g_wgsize);
        if(wzxd>=0 && wzxd<=31 && wzyd>=0 && wzyd<=31)
        {
            wz_x=wzxd,wz_y=wzyd;
            if(lx)
            {
                huaxian(wz_x,wz_y);
                l_x = (int8)wz_x,l_y = (int8)wz_y;
            }
            else g_color = ems_data[wz_x][wz_y];
            chonghua(FALSE);
            revColor(0);
        }
        return 0;
    }
    case WM_MOUSEDOWN:
    {
        int p1 = (int)wParam;
        int p2 = (int)lParam;

        //��ɫ
        if(IsPointInObject(p1,p2,&gColor))
        {
            revColor(1);  
            return 0;//��Ҫ��ͼȥ�Ż����case�е���Щreturn
        }//����
        else if(IsPointInObject(p1,p2,&gWangge))
        {
            g_wangge = !g_wangge;
            MainShow(TRUE);//��Ϊ������������Ҫȫ���ػ�
            return 0;

        }//�Ƿ�����
        else if(IsPointInObject(p1,p2,&gLx))
        {
            lx = !lx;
            drawState(1);
            return 0;
        }//Ԥ��
        else if(IsPointInObject(p1,p2,&gYulan))
        {
            ShowPREVIEW(TOPWND_MAINWND);
            return 0;
        }

        //ѡ����ƶ�(����)
        wzxd=(int8)((p1-8)/g_wgsize), wzyd=(int8)((p2-8)/g_wgsize);
        if(wzxd>=0 && wzxd<=31 && wzyd>=0 && wzyd<=31)
        {
            wz_x=wzxd,wz_y=wzyd;
            if(lx)
            {
                mrc_memcpy(ems_snap,ems_data,1024);//�������
                l_x = (int8)wz_x, l_y = (int8)wz_y;
                ems_data[wz_x][wz_y] = (uint8)g_color;
            }else
                g_color = ems_data[wz_x][wz_y];
            chonghua(FALSE);
            revColor(0);
        }
        return 0;
    }
    case WM_KEYDOWN:
    {
        switch(wParam)
        {
        case MR_KEY_1://�ı���ɫ
            revColor(1);
            return 1;	
        case MR_KEY_3://�Ƿ���������
            lx = !lx;
            drawState(1);
            return 1;
        case MR_KEY_7://�Ƿ���ʾ����
            g_wangge = !g_wangge;
            MainShow(TRUE);
            return 1;
        }

        //ע�����ﲻ��Ҫbreak������
    }
    case WM_KEYDOWNREPEAT:
	{
        switch(wParam)
        {
        case MR_KEY_2://�Ϸ���
        case MR_KEY_UP:
            if(wz_y>0) wz_y--;
            else wz_y=31;
            break;
        case MR_KEY_8://�·���
        case MR_KEY_DOWN:
            if(wz_y<31)	wz_y++;
            else wz_y=0;
            break;
        case MR_KEY_4://����
        case MR_KEY_LEFT:
            if(wz_x>0) wz_x--;
            else wz_x = 31;
            break;
        case MR_KEY_6://�ҷ���
        case MR_KEY_RIGHT:
            if(wz_x<31) wz_x++;
            else wz_x=0;
            break;
        case MR_KEY_5:
        case MR_KEY_SELECT:
            revColor(1);
            return 1;
        }
        if(lx) ems_data[wz_x][wz_y] = g_color;
        else g_color = ems_data[wz_x][wz_y];
        chonghua(FALSE);
        revColor(0);
        return 1;
	}
	case WM_KEYUP:
    {
        switch(wParam)
        {
        case MR_KEY_SOFTRIGHT:
            if(_IS_SET_STATE(HUATU))//�ڻ�ͼ״̬(��Բ,��...,�˰���Ϊȡ��)�����ָ�toolbar���ַ�
            {
                goto label1;
            }
            HideTopWindow(TOPWND_MAINWND, FALSE, FALSE);
            ShowTopWindow(TOPWND_START, NULL, 0);
            return 1;
        case MR_KEY_SOFTLEFT://�˵�
            if(_IS_SET_STATE(HUATU))//�Ƿ��ǻ�ͼ״̬����ͼ���������֣�
            {
                if(_IS_SET_STATE(HUATEXT))
                {
                    CharToEmsOk();
                    goto label1;
                }
                else if(_IS_SET_STATE(HUATUPOINTOK))//�Ѿ�ȡ�õ�λ��,���Խ��л�ͼ��
                {
                    if(_IS_SET_STATE(HUAX))//����(���밴˳����,�Ȼ���,��Բ...)
                    {
                        EmsDrawLine(lastx,lasty,wz_x,wz_y);
                    }
                    else if(_IS_SET_STATE(HUAYUAN))//��Բ
                    {
                        int16 t1,t2;

                        t1 = wz_x > lastx ? (wz_x - lastx) : (lastx - wz_x);
                        t2 = wz_y > lasty ? (wz_y - lasty) : (lasty - wz_y);
                        EmsDrawCircle(lastx,lasty,(t1>t2?t1:t2));
                    }
                    else if(_IS_SET_STATE(HUAJUXING))//������
                    {
                        EmsDrawRect(lastx,lasty,wz_x,wz_y,TRUE);
                    }
                    else if(_IS_SET_STATE(HUAXIANKUANG))//���߿�
                    {
                        EmsDrawRect(lastx,lasty,wz_x,wz_y,FALSE);
                    }
label1:
                    SMP_Toolbar_SetStrings(hToolbar, STR_MENU, RESID_INVALID, STR_RETURN, 1);
                    _CLR_STATE(0xffff);//��ͼ��ɣ��������״̬
                    gIsCharToEms=FALSE;
                    MainShow(TRUE);//��ͼ��ɣ�ȫ���ػ�
                    return 1;
                }
                else//��һ��ȡ�����ʾȡ�ڶ���
                {
                    lastx=wz_x,lasty=wz_y;
                    _SET_STATE(HUATUPOINTOK);
                    SMP_MsgBox(0,hWnd,NULL,SGL_LoadString(STR_MSG),SGL_LoadString(STR_SECONDPOINT),ID_OK,NULL);
                }
            }
            else ShowOptMenu(hWnd);
            return 1;
        case MR_KEY_9://Ԥ��
            ShowPREVIEW(TOPWND_MAINWND);
            return 1;
        }
        break;
	}
	case WM_COMMAND:
 	{
        command(hWnd,Msg,wParam,lParam);
		return 0;
 	}
	}
    return SMP_MenuWnd_WndProc(hToolbar,hWnd, Msg, wParam, lParam);
}

