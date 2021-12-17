#ifndef MD5_H
#define MD5_H

#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <cstdio>

#ifdef _WIN32
#pragma once
#endif

#define MD5_DIGEST_LENGTH 16  // 16 bytes == 128 bit digest

// MD5 Hash
typedef struct
{
	unsigned int buf[ 4 ];
	unsigned int bits[ 2 ];
	unsigned char in[ 64 ];
} iMD5Context_t;

void MD5Init( iMD5Context_t *context );
void MD5Update( iMD5Context_t *context, unsigned char const *buf, unsigned int len );
void MD5Final( unsigned char digest[ MD5_DIGEST_LENGTH ], iMD5Context_t *context );
char *MD5_Print( unsigned char digest[ MD5_DIGEST_LENGTH ] );
void MD5Transform( unsigned int buf[ 4 ], unsigned int const in[ 16 ] );
unsigned int MD5_PseudoRandom( unsigned int nSeed );
#endif // CHECKSUM_MD5_H