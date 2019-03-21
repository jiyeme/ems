
#include "mrc_base.h"
#include "window.h"
#include "basic.h"

/////////////////////////---�궨�弰��������---///////////////////////////////////
#define NUM_LAB  30            /*  label �ĸ��� */
#define LAB_LEN  5             /*  label �ı����� */
#define FOR_NEST 5
#define SUB_NEST 10            /* �ӳ��������� */


typedef enum E_TOKEN_TYPE{
	DELIMITER = 1,   /* ������� */
	VARIABLE,	     /* ���� */
	NUMBER,		     /* ���� */
	COMMAND,		 /* ���� */
	STRING,		     /* �ַ��� */
    UNKNOWN
}E_TOKEN_TYPE;

typedef enum E_YUJU_TYPE{
	YUJU_TRICE = 1,
    YUJU_CLS,
    YUJU_REF,
    YUJU_BMP,
    YUJU_POINT,
    YUJU_LINE,
    YUJU_RECT,
    YUJU_SRECT,
    YUJU_MKDIR,
    YUJU_REMOVE,
    YUJU_WRITE,
    YUJU_TXT,
    YUJU_FILESTATE, /* �������� */
	YUJU_IF,
	YUJU_THEN,
    YUJU_GOTO,
	YUJU_FOR,
	YUJU_NEXT,
	YUJU_TO,
	YUJU_GOSUB,
	YUJU_RET,
	YUJU_END,
    YUJU_CMDEND,      /* ���� keep!*/ 
	YUJU_EOL,         /* �н�����־ ������һ����� */
	YUJU_FINISHED     /* �ļ�������־ */
}E_YUJU_TYPE;

typedef struct 
{
    int var;           /* �������������� */
    int target;        /* Ŀ��ֵ */
    char *loc;
}FOR_STACK;



/////////////////////////---��������---///////////////////////////////////


static int level2(int *ret);


/////////////////////////---ȫ�ֱ���---///////////////////////////////////

static struct commands/* �ؼ��ֲ��ұ� */
{
    char command[6];
    E_YUJU_TYPE yuju_type;
}const table[] ={              /* �����������Сд */
    "if", YUJU_IF,
    "then", YUJU_THEN,
    "goto", YUJU_GOTO,
    "for", YUJU_FOR,
    "next", YUJU_NEXT,
    "to", YUJU_TO,
    "gosub", YUJU_GOSUB,
    "ret", YUJU_RET,
    "end", YUJU_END,
    //��������
    "trice", YUJU_TRICE,
    "cls", YUJU_CLS,
    "ref",YUJU_REF,
    "bmp",YUJU_BMP,
    "point",YUJU_POINT,
    "line",YUJU_LINE,
    "rect",YUJU_RECT,
    "srect",YUJU_SRECT,
    "mkdir",YUJU_MKDIR,
    "del",YUJU_REMOVE,
    "write",YUJU_WRITE,
    "txt",YUJU_TXT,
    "testf",YUJU_FILESTATE 
};

static struct 
{
	char name[LAB_LEN];
	char *p;            /* ָ���λ�� */
}label[NUM_LAB];        /* ��ű� */

static int variables[26];            /* 26���û�����, A-Z (��Ҫȫ����ʼ��Ϊ0!)*/
static int ftos;                     /* FORջ������ */
static int gtos;                     /* GOSUBջ������ */
static FOR_STACK fstack[FOR_NEST];   /* FOR/NEXTѭ��ջ */
static char *gstack[SUB_NEST];       /* gosubջ */
static char *token;              /* ��ǰ����ĵ��� */
static E_TOKEN_TYPE token_type;      /* ��ǰ����ĵ��ʵ����� */
static E_YUJU_TYPE yuju_type;        /* ��ǰ������������� */
static char *prog;                   /* ������Ҫ�����ı��ʽ */


/////////////////////////---����ʵ��---///////////////////////////////////

static void nextline(void)
{
    while((*prog!='\n') && *prog) ++prog;
    if(*prog) ++prog;//��������ļ�������*progָ����һ�еĿ�ʼ
}
static void putback()/* �����ŷ��ص������� */
{
    char *t = token;
    for(; *t; t++) prog--;
}

static int get_token(void)/* ȡ���� */
{
	char *temp=token;

    yuju_type = 0;
    token_type = DELIMITER;
    *token = '\0';
start:
	if(!*prog) /* �ļ����� */
	{
		yuju_type = YUJU_FINISHED;
		return token_type;
	}

	while(iswhite(*prog))/* �����հ� */ 
        ++prog;

    if('\r' == *prog) /* �س� */
	{
        mrc_strcpy(token,"\r\n");
		prog+=2;
		yuju_type = YUJU_EOL;
		return token_type;
	}
    if(*prog == '/')/* ����ע�� */
    {
        if(prog[1] == '/')
        {
            while(*prog != '\r' && *prog)
                prog++;
            goto start;
        }
    }
	if(mrc_strchr("+-/%*=;(),><", *prog)) /* ���� */
	{
		*temp = *prog;
		++prog;
		++temp;
		*temp = '\0';
		return token_type;
	}

	if('"' == *prog) /* ���ַ���(������) */
	{
		++prog;
		while(*prog != '"') 
        {
            *temp = *prog;
            ++prog;
            ++temp;
        }
		++prog;
		*temp='\0';
        token_type = STRING;
		return token_type;
	}

	if(isdigit(*prog)) /* ���� */
	{
		while(isdigit(*prog))
        {
            *temp = *prog;
            ++temp;
            ++prog;
        }
		*temp = '\0';
        token_type = NUMBER;
		return token_type;
	}

	if(isalpha(*prog)) /* ���������� */
    {
        int i=0;
        while(isalpha(*prog) || isdigit(*prog))
        {
            *temp = *prog;
            ++temp;
            ++prog;
        }
        *temp = '\0';
        yuju_type = 0;/* ������Ϊ��֪�������� */	  
	    while(token[i]){ token[i] = tolower(token[i]); ++i; }/* ת��ΪСд */
	    
	    for(i=0; i<YUJU_CMDEND; ++i)/* �ؼ��ֲ��ұ��в��� */
        {
            if(!mrc_strcmp(table[i].command, token)) 
            {
                yuju_type = table[i].yuju_type;/* �ҵ�֮���ȡ������� */
                break;
            }
        }    
		if(!yuju_type)
            token_type = VARIABLE;/* δ�ҵ����Ǳ��� */
		else
            token_type = COMMAND;
        return token_type;
    }	
	return token_type=UNKNOWN;
}
static void arith(char o, int *r, int *h)/* ִ��ָ������������ */
{
    switch(o)
    {
    case '-':
        *r = *r-*h; 
        return;
    case '+':
        *r = *r+*h; 
        return;
    case '*':
        *r = (*r)*(*h); 
        return;
    case '/':
        if(!*h) return;
        *r = (*r)/(*h);
        return;
    case '%':
        *r = *r % *h; 
        return;
    }
}

static int primitive(int *ret)/* ���ұ���������ֵ */
{
    switch(token_type)
    {
    case VARIABLE: /* ���ұ��� */
        {
            *ret = variables[toupper(*token)-'A'];
            get_token(); 
            break;
        }
    case NUMBER:
        *ret = mrc_atoi(token);
        get_token(); 
        break;
    default:
        return -1;
    }
    return 0;
}

static int level5(int *ret)/* �������ű��ʽ */
{
	if(*token == '(')
	{
		get_token(); 
		if(level2(ret))
            return -1; 
		if(*token != ')')
		    return -1;
		get_token(); 
	}
	else
    {
		if(primitive(ret))
            return -1;        
    }
    return 0;
}

static int level4(int *ret)/* �ǵ�һ�� + �� - */
{
	char op = '\0'; 

	if(*token=='+' || *token=='-')
	{
		op = *token; 
		get_token(); 
    }
    if(level5(ret))
        return -1; 
    if(op == '-')
        *ret = -(*ret);
    return 0;
}

static int level3(int *ret)/* �˻���������� */
{
	if(level4(ret))
        return -1; 
	while(*token == '*' || *token == '/' || *token == '%')
	{
        char op = *token; 
        int hold=0;

		get_token(); 
		if(level4(&hold))
            return -1; 
		arith(op, ret, &hold); 
	}
    return 0;
}

static int level2(int *ret)/* �ӻ������ */
{
	if(level3(ret))
        return -1; 
	while(*token == '+' || *token == '-')
	{
        char op = *token; 
        int hold=0;

		get_token(); 
		if(level3(&hold))
            return -1; 
		arith(op, ret, &hold);
	}
    return 0;
}

static int get_exp(int *ret)/* ��������� */
{           
	get_token();
    if(!*token)
        return -1;
    if(level2(ret))
        return -1;
    putback();
    return 0;
}

static int assignment(void)/* ָ��һ��������ֵ */
{
	int var, value=0;

    get_token();
    if(VARIABLE == token_type)
    {
        var = toupper(*token)-'A';
        get_token();
        if(*token != '=')
            return -1;
        if(get_exp(&value))
            return -1;
        variables[var] = value;
    }
    return 0;
}


static int ec_goto(void)/* ִ��GOTO��� */
{
	char *loc = NULL;
    int i;

	get_token();
	for(i=0; i<NUM_LAB; ++i) /* ���ұ��ر�ǩ�� */
		if(!mrc_strcmp(label[i].name,token))
			loc = label[i].p;

	if(!*loc)
        return -1;
	else
        prog = loc; /* �����LOC����ʼ���� */
    return 0;
}

static int ec_if(void)/* ִ��IF��� */
{
    int x=0, y=0, cond=0;
    char op;

    if(get_exp(&x))/* ��������ʽ */
        return -1;
    get_token(); /* ȡ������ */     
    if(!mrc_strchr("=<>", *token))
        return -1;
    op = *token;

    if(get_exp(&y)) /* ��������ʽ */
        return -1; 

    /* ȷ����� */
    switch(op)
    {
    case '<':
        if(x < y) cond = 1;
        break;
    case '>':
        if(x > y) cond = 1;
        break;
    case '=':
        if(x == y) cond = 1;
        break;
    }
    if(cond) /* ��������Ϊ�� */
    {
        get_token();
        if(yuju_type != YUJU_THEN)
            return -1;
    }
    else
        nextline();
    return 0;
}

static int fpush(FOR_STACK *i)/* FOR��ջѹջ */
{
    if(ftos<FOR_NEST)
    {
        fstack[ftos]=*i;
        ++ftos;
        return 0;
    }
    return -1;
}

static int ec_for(void)/* ִ��FORѭ�� */
{
	FOR_STACK i;
	int value=0;

	get_token(); 
	if(!isalpha(*token))
		return -1;

	i.var = toupper(*token)-'A'; /* �������� */

	get_token(); 
	if(*token != '=')
		return -1;

	if(get_exp(&value))/* ȡ��ʼ��ֵ */
        return -1; 

	variables[i.var] = value;/* ������ָ��ֵ */

	get_token();
	if(yuju_type != YUJU_TO)
        return -1;
	if(get_exp(&i.target))/* ȡ����ֵ */
        return -1; 

	/* ���ѭ����ִ������һ��, ѹջ */
	if(i.target >= value)
	{
		i.loc = prog;
		if(fpush(&i))
            return -1;
	}
	else /* ������������ѭ������ */
		while(yuju_type != YUJU_NEXT) 
            get_token();
    return 0;
}


static int ec_next(void)/* ִ��NEXT��� */
{
	FOR_STACK i;

    --ftos;/* FOR��ջ��ջ */
	if(ftos<0)
	    return -1;
    i = fstack[ftos];/* ȡѭ����Ϣ */

	variables[i.var]++; /* �������Ʊ��� */
	if(variables[i.var]>i.target) /* ѭ������ */
        return 0; 
	if(fpush(&i))/* ����洢ѭ����Ϣ */
        return -1; 
	prog = i.loc; /* ����ѭ�� */
    return 0;
}

static int ec_gosub(void)/* ִ��GOSUB���� */
{
	char *loc=NULL;
    int i;

	get_token();
    for(i=0; i<NUM_LAB; ++i) /* ���ұ�ǩ */
	if(!mrc_strcmp(label[i].name,token))
		loc = label[i].p;

	if(!loc)
        return -1;
    else
	{   
	    gtos++;
	    if(gtos==SUB_NEST)
		    return -1;
	    gstack[gtos] = prog;/* Ϊ���ر��浱ǰ�� */
		prog = loc; /* ��loc��ʼִ�г��� */
	}
    return 0;
}



static int scan_labels(void)/* �������б�ǩ */
{
	signed int addr=0;
	char *temp;
    int i;

	temp = prog;    /* ����Դ�ļ���ָ�� */
	for(i=0; i<NUM_LAB; ++i)/* ��ʼ����ǩ�ַ���,���涨, ��ǩ��Ϊ�ձ�ʾ�ñ�ǩ����δʹ��,ʹ���ж��ı�ǩΪ�� */
		label[i].name[0]='\0';

	do
	{
    	get_token();
		if(token_type == NUMBER)
		{        
	        addr = -1;//��ǩ����
	        for(i=0; i<NUM_LAB; ++i)
	        {
		        if(!label[i].name[0])//�Ƿ�Ϊδʹ��
                {
                    addr = i;//��ǩ�����λ�õ�����
                    break;
                }
		        if(!mrc_strcmp(label[i].name,token)) 
                {
                    addr = -2;//�ҵ���ͬ��ǩ
                    break;
                }
	        }

			if(-1 == addr || -2 == addr)
                return -1;
            mrc_strcpy(label[addr].name, token);
            label[addr].p = prog;
        }
        if(yuju_type != YUJU_EOL) /* �������һ���հ��У�����һ�� */
            nextline();
    }while(yuju_type!=YUJU_FINISHED);
    prog = temp; /* �ָ�Դ�ļ���ָ�� */
    return 0;
}

static int GetNumPram(char *str, int pram[], int pramlen)
{
    if(str!=NULL)/* ����ַ��� */
    {
        get_token();
        if(STRING != token_type)
            return -1;
        mrc_strcpy(str,token);
        get_token();
    }
    if(pram!=NULL)// �����ֵ
    {
        int answer=0,i=0;

        do
        {
            if(get_exp(&answer)) return -1;
            pram[i]=answer; 
            i++;
            get_token();
        }while(',' == *token && i<pramlen);
    }
    if(YUJU_EOL != yuju_type && YUJU_FINISHED != yuju_type)
        return -1;
    return 0;
}

///////////////////--�⺯��--/////////////////////////
static int ec_trice(void)
{
	int answer=0;
	int len=0, spaces=0;
	char m_last_delim='\0';
    char tmp[30];
    int32 h31415926 = mrc_open("trice.txt", MR_FILE_CREATE|MR_FILE_WRONLY);

    if(!h31415926) return -1;
    mrc_seek(h31415926, 0, MR_SEEK_END);
    do
	{
		get_token();
        if(YUJU_EOL == yuju_type || YUJU_FINISHED == yuju_type) break;
		if(STRING == token_type) /* ���ַ��� */
		{
			mrc_write(h31415926,token,strlen(token));
			len += mrc_strlen(token);
			get_token();
		}
		else if(token_type == VARIABLE || token_type == NUMBER) /* �����ֻ���� */
        {
            putback();
            if(get_exp(&answer)) goto err;
            mrc_sprintf(tmp,"%d", answer);
            len += mrc_strlen(tmp);
            mrc_write(h31415926,tmp,mrc_strlen(tmp));
			get_token();
		}
		m_last_delim = *token; 

		if(';' == *token)
		{
			spaces = 8 - (len % 8); 
            len += spaces; /* ����Ʊ���ռ� */
            while(spaces)
            {
                mrc_write(h31415926," ",1);
                --spaces;
            }
        }
        else if(',' == *token) ;/* �����κδ��� */
        else if(YUJU_EOL != yuju_type && YUJU_FINISHED != yuju_type) goto err;

    }while(';' == *token || ',' == *token);

	if(YUJU_EOL == yuju_type || YUJU_FINISHED == yuju_type)
	{
		if(';' != m_last_delim && ',' != m_last_delim)
             mrc_write(h31415926,"\r\n",2);
	}
	else
        goto err; 
    mrc_close(h31415926);
    return 0;
err:
    mrc_close(h31415926);
    return -1;
}


static int ec_cls(void)
{
    int pram[3];

    if(GetNumPram(NULL,pram,3))
        return -1;
    mrc_clearScreen(pram[0],pram[1],pram[2]);
    return 0;
}
static int ec_ref(void)
{
    int pram[4];

    if(GetNumPram(NULL,pram,4))
        return -1;
    mrc_refreshScreen(pram[0],pram[1],pram[2],pram[3]);
    return 0;
}
static int ec_bmp(void)
{
    int pram[7];
    char name[32];

    if(GetNumPram(name,pram,7))
        return -1;

    mrc_bitmapNew(0,pram[5],pram[6]);
    mrc_bitmapLoad(0,name,0,0,pram[5],pram[6],pram[5]);
    mrc_bitmapShow(0,pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6]);
    mrc_bitmapLoad(0,"*",0,0,pram[5],pram[6],pram[5]);
    return 0;
}

static int ec_write(void)
{
    char temp[256];
    int32 f,filelen;
    uint8 *filebuf=NULL;

    get_token();
    if(token_type != STRING) return -1;
    mrc_strcpy(temp,token);
    get_token();
    if(*token != ',') return -1;
    get_token();
    if(token_type != STRING) return -1;

    if(mrc_fileState(token) == MR_IS_INVALID)
    {
        if(mrc_readFileFromMrpEx(NULL,temp, &filebuf, &filelen,0))
            return -1;
        f = mrc_open(token,MR_FILE_RDWR|MR_FILE_CREATE);
        if(f) {
            mrc_write(f,filebuf,filelen);
            mrc_close(f);
        }
        free(filebuf);
    }
    return 0;
}

static int ec_srect(void)
{
    int pram[11];

    if(GetNumPram(NULL,pram,11))
        return -1;
    ShadeRect(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6],pram[7],pram[8],pram[9],pram[10]);
    return 0;
}

static int ec_point(void)
{
    int pram[5];

    if(GetNumPram(NULL,pram,5))
        return -1;
    mrc_drawPointEx(pram[0],pram[1],pram[2],pram[3],pram[4]);
    return 0;
}
static int ec_line(void)
{
    int pram[7];

    if(GetNumPram(NULL,pram,7))
        return -1;
    mrc_drawLine(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6]);
    return 0;
}
static int ec_rect(void)
{
    int pram[7];

    if(GetNumPram(NULL,pram,7))
        return -1;
    mrc_drawRect(pram[0],pram[1],pram[2],pram[3],pram[4],pram[5],pram[6]);
    return 0;
}
static int ec_txt(void)
{
    mr_screenRectSt rect;
    mr_colourSt color;
    int pram[7];
    char tmp[256];

    if(GetNumPram(tmp,pram,7))
        return -1;
    rect.x=pram[0];
    rect.y=pram[1];
    rect.w=pram[2];
    rect.h=pram[3];
    color.r=pram[4];
    color.g=pram[5];
    color.b=pram[6];
    mrc_drawTextEx(tmp,rect.x,rect.y,rect,color,DRAW_TEXT_EX_IS_AUTO_NEWLINE,MR_FONT_MEDIUM);
    return 0;

}
static int ec_mkdir(void)
{
    char tmp[128];
    if(GetNumPram(tmp,NULL,0))
        return -1;
    variables['r'-'a'] = mrc_mkDir(tmp);
    return 0;
}

static int ec_remove(void)
{
    char tmp[128];
    if(GetNumPram(tmp,NULL,0))
        return -1;
    if(mrc_fileState(tmp)==MR_IS_DIR)
        variables['r'-'a'] = removeDir(tmp);
    else
        variables['r'-'a'] = mrc_remove(tmp);
    return 0;
}

static int ec_filestate(void)
{
    char tmp[128];

    if(GetNumPram(tmp,NULL,0))
        return -1;
    variables['r'-'a'] = mrc_fileState(tmp);
    return 0;
}

//////////////////////////////////////////////////////
int StartBasic(char *mem)
{
    int ret=0;
    extern uint16 SCREEN_WIDTH;	
    extern uint16 SCREEN_HEIGHT;

    prog = mem;
	mrc_memset(variables,0,sizeof(variables));
    token=malloc(255);
    if(!token) return -1;
    mrc_memset(token,0,255);
    mrc_memset(gstack,0,sizeof(gstack));

    gtos = ftos = token_type = yuju_type = 0;
    variables['w'-'a']=SCREEN_WIDTH;
    variables['h'-'a']=SCREEN_HEIGHT;
    GetMediumFontWH((int32*)&variables[0],(int32*)&variables[1],(uint16)SGL_GetSystemFont());
	if(scan_labels()) goto err;
	do
	{
		get_token();
		if(VARIABLE == token_type)
        {
            putback();
            if(assignment())/* �����Ǹ�ֵ��� */
                goto err;
        }
        else if(COMMAND == token_type)
        {
            switch(yuju_type)
            {
            case YUJU_TRICE:
                ret = ec_trice();
                break;
            case YUJU_CLS:
                ret = ec_cls();
                break;
            case YUJU_REF:
                ret = ec_ref();
                break;
            case YUJU_BMP:
                ret = ec_bmp();
                break;
            case YUJU_POINT:
                ret = ec_point();
                break;
            case YUJU_LINE:
                ret = ec_line();
                break;
            case YUJU_RECT:
                ret = ec_rect();
                break;
            case YUJU_SRECT:
                ret = ec_srect();
                break;
            case YUJU_MKDIR:
                ret = ec_mkdir();
                break;
            case YUJU_REMOVE:
                ret = ec_remove();
                break;
            case YUJU_WRITE:
                ret = ec_write();
                break;
            case YUJU_TXT:
                ret = ec_txt();
                break;
            case YUJU_FILESTATE:
                ret = ec_filestate();
                break;
            case YUJU_GOTO:
                ret = ec_goto();
                break;
            case YUJU_IF:
                ret = ec_if();
                break;
            case YUJU_FOR:
                ret = ec_for();
		    	break;
    	    case YUJU_NEXT:
                ret = ec_next();
		    	break;
    	    case YUJU_GOSUB:
                ret = ec_gosub();
		    	break;
		    case YUJU_RET:              
                if(!gtos)/* GOSUB��ջ */
		            goto err;
                else
                    prog = gstack[gtos--];/* ��GOSUB���� */
			    break;
		    case YUJU_END:
                return 0;
		    }
            if(ret) goto err;
        }
	}while(yuju_type != YUJU_FINISHED);
    return 0;
err:
    free(token);
    return -1;
}