#ifndef TOMCRYPT_H_
#define TOMCRYPT_H_

#ifdef __cplusplus
extern "C" {
#endif

/* ������� */
enum {
   CRYPT_OK=0,             /* ����ɹ�*/
   CRYPT_ERROR,            /* һ�����*/
   CRYPT_NOP,              /* Not a failure but no operation was performed ����ʧ�ܣ���û��ִ�в���*/

   CRYPT_INVALID_KEYSIZE,  /* �������Կ��С��Ч */
   CRYPT_INVALID_ROUNDS,   /* Invalid number of rounds��Ч�ĺ��� */
   CRYPT_FAIL_TESTVECTOR,  /* Algorithm failed test vectors �㷨δ�ܲ�������*/

   CRYPT_BUFFER_OVERFLOW,  /* û���㹻�Ŀռ���� */
   CRYPT_INVALID_PACKET,   /* ��Ч���������ݰ� */

   CRYPT_INVALID_PRNGSIZE, /* Invalid number of bits for a PRNG λ��Ч��PRNG����*/
   CRYPT_ERROR_READPRNG,   /* Could not read enough from PRNG �޷���ȡ�㹻��PRNG��*/

   CRYPT_INVALID_CIPHER,   /* Invalid cipher specified ��Ч������ָ��*/
   CRYPT_INVALID_HASH,     /* Invalid hash specified ��Ч�Ĺ�ϣָ��*/
   CRYPT_INVALID_PRNG,     /* Invalid PRNG specified ��Ч��PRNG��ָ��*/

   CRYPT_MEM,              /* �ڴ治��*/

   CRYPT_PK_TYPE_MISMATCH, /* Not equivalent types of PK keys ��PK�Ĳ��ȼ�����*/
   CRYPT_PK_NOT_PRIVATE,   /* Requires a private PK key ��Ҫһ��˽�˵�PK��*/

   CRYPT_INVALID_ARG,      /* Generic invalid argument ͨ����Ч�Ĳ���*/
   CRYPT_FILE_NOTFOUND,    /* �ļ�δ�ҵ�*/

   CRYPT_PK_INVALID_TYPE,  /* Invalid type of PK key ��Ч��PK�ͼ�*/
   CRYPT_PK_INVALID_SYSTEM,/* Invalid PK system specified ��Ч��PKϵͳָ��**/
   CRYPT_PK_DUP,           /* Duplicate key already in key ring �ظ����Ѿ���Կ��Ȧ*/
   CRYPT_PK_NOT_FOUND,     /* Key not found in keyring ��δ�ҵ�Կ��Ȧ*/
   CRYPT_PK_INVALID_SIZE,  /* Invalid size input for PK parameters ��Ч��PK����*�ߴ�����*/

   CRYPT_INVALID_PRIME_SIZE,/* Invalid size of prime requested��Ч����Ļƽ�ߴ� */
   CRYPT_PK_INVALID_PADDING /* Invalid padding on input ��Ч�����������*/
};


/**
   base64 Encode a buffer (NUL terminated)
   @param in      The input buffer to encode
   @param inlen   The length of the input buffer
   @param out     [out] The destination of the base64 encoded data
   @param outlen  [in/out] The max size and resulting size
   @return CRYPT_OK if successful
*/
/**
   Base64���루��NUL��ֹ��
   @�����뻺�����������б���
   @����inlen�����뻺�����ĳ���
   @��������[���]��Base64��������Ŀ��
   @����outlen [����/���]���ɴ˲����Ĺ�ģ���ߴ�
   ����ɹ�����CRYPT_OK @
*/

int base64_encode(const unsigned char *in,  unsigned long inlen, 
                        unsigned char *out, unsigned long *outlen);


/**
   Base64����

   ����in         base64�Ľ�������
   ����inlen      base64�����ݳ���
   ����out        [���]����Ķ���������
   ����outlen     [����/���]���ߴ��Լ��ɴ˲����Ľ������ݵĴ�С
   ����ɹ�, ����CRYPT_OK
*/
int base64_decode(const unsigned char *in,  unsigned long inlen, 
                        unsigned char *out, unsigned long *outlen);

#ifdef __cplusplus
   }
#endif

#endif /* TOMCRYPT_H_ */
