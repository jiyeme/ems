#ifndef _MD5_H
#define _MD5_H

#include "types.h"

/*  
 * \��MD5�㷨ȡɢ��ֵ
 * \����szSour			Դ�ַ���
 * \����iLen			Դ�ַ����ĳ���
 * \����[���]szDest	���16���ֽ�
 */

VOID MD5(BYTE *szSour, int iLen, BYTE *szDest);

#endif /*_MD5_H*/