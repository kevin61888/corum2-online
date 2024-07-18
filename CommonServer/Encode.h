#ifndef __ENCODE_H__
#define __ENCODE_H__


#pragma once


#include "CommonHeader.h"


#define ENCODE_KEY			"ÎýÜµâ³ô¸î¦,ÍéëâØ¿Ø¿á¨"
#define ENCODE_SUBKEY		3
#define DECODE_KEY			ENCODE_KEY
#define DECODE_SUBKEY		ENCODE_SUBKEY
#define MAX_READING			40960


BOOL DECLSPECIFIER EncodeINIFile(char* szSrcFileName, char* szSaveFile, char* szEncodeKey, int nEncodeSubKey);
BOOL DECLSPECIFIER DecodeCFGFile(char* szLoadFile, char* szTagetFile, char* szDecodeKey, int nDecodeSubKey);
void DECLSPECIFIER CreateRandomString(IN OUT char* szString, int iLen);


#endif 