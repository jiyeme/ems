#ifndef _CONFIG_H
#define _CONFIG_H

/**
 * \����Ϊ����ֵ����ʾϵͳ�Ǵ��
 */
#ifndef BIG_ENDIAN
    #ifdef MR_SPREADTRUM_MOD
        #define BIG_ENDIAN	1
    #else
        #define BIG_ENDIAN	0
    #endif
#endif

/**
 * \����Ϊ����ֵ����ʾϵͳ��С��
 */
#ifndef LITTLE_ENDIAN
    #ifdef MR_SPREADTRUM_MOD
        #define LITTLE_ENDIAN	0
    #else
        #define LITTLE_ENDIAN	1
    #endif
#endif

/**
 * \�����������꣬����ģ�齫���ᱻ����
 */
#define OMIT_NETWORK_MODULE 1
#ifndef OMIT_NETWORK_MODULE
#define OMIT_NETWORK_MODULE	0
#endif

/**
 * \�����������꣬WM_KEYDOWNREPEAT�¼����á�
 */
#ifndef ENABLE_SGL_KEYREPEAT
#define ENABLE_SGL_KEYREPEAT
#endif


#endif //_CONFIG_H

