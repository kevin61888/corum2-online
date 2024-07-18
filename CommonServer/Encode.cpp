#include "stdafx.h"
#include "Encode.h"


BOOL DECLSPECIFIER EncodeINIFile(char* szSrcFileName, char* szSaveFile, char* szEncodeKey, int nEncodeSubKey)
{
	FILE*	fp, *fp1;
	fp = fopen( szSaveFile, "wb" );
	if( !fp ) return FALSE;
	
	fp1 = fopen( szSrcFileName, "rb" );
	if( !fp1 ) return FALSE;
	
	int nKeyLen = strlen(szEncodeKey);
	
	DWORD nCurWrite = 0;
	int nRemainWrite = 0;

	char szBuf[MAX_READING]={0,};
	fread(szBuf, MAX_READING, 1, fp1);

	DWORD dwTotalDataSize = strlen(szBuf);
	fwrite(&dwTotalDataSize, sizeof(DWORD), 1, fp );

	while(nCurWrite < dwTotalDataSize)
	{
		BYTE* szData = (BYTE*)(szBuf + nCurWrite);
	
		nRemainWrite = dwTotalDataSize - nCurWrite;	//한행에서 앞으로 읽어야할 남은 데이터 Size
		if(nRemainWrite < nKeyLen)
		{
			for(int k=0; k<nRemainWrite; k++ )
				szData[k] ^= (szEncodeKey[k] + nEncodeSubKey);

			fwrite(szData, nRemainWrite, 1, fp );
			nCurWrite += nRemainWrite;
		}
		else
		{
			for(int k=0; k<nKeyLen; k++ )
				szData[k] ^= (szEncodeKey[k] + nEncodeSubKey);
			
			fwrite(szData, nKeyLen, 1, fp );
			nCurWrite += nKeyLen;
		}
	}

	fclose( fp );
	fclose( fp1 );

	return TRUE;
}

BOOL DECLSPECIFIER DecodeCFGFile(char* szLoadFile, char* szTagetFile, char* szDecodeKey, int nDecodeSubKey)
{
	FILE*	fp = NULL;	
	FILE*	fp1 = NULL;
	DWORD	dwCur = 0;
	DWORD   dwTotalLen = 0;
	BOOL	bRet = TRUE;
	
	int nKeyLen = strlen(szDecodeKey);

	fp = fopen( szLoadFile, "rb" );
	if(!fp) return FALSE;

	fp1 = fopen( szTagetFile, "wb" );
	if( !fp1 ) return FALSE;

	fread(&dwTotalLen, sizeof(DWORD), 1, fp );

	char* szBuffer = new char[ dwTotalLen ];
	
	while( bRet )
	{
		if(!fread(szBuffer + dwCur, nKeyLen, 1, fp ))
			bRet = FALSE;

		if(bRet)
		{
			for(int k=0; k<nKeyLen; k++ )
				szBuffer[ dwCur + k ] ^= (szDecodeKey[k] + nDecodeSubKey);

			dwCur += nKeyLen;
		}
		else
		{
			int nRemain = dwTotalLen - dwCur;
			for(int k=0; k<nRemain; k++ )
				szBuffer[ dwCur + k ] ^= (szDecodeKey[k] + nDecodeSubKey);

			dwCur += nRemain;
		}
	}

	fwrite(szBuffer, dwTotalLen, 1, fp1);

	delete [] szBuffer;

	fclose(fp1);
	fclose(fp);

	return TRUE;
}

// 10자의 임의의 영문 문자열 생성.
void DECLSPECIFIER CreateRandomString(IN OUT char* szString, int iLen)
{
	iLen;
	srand((unsigned int) time(0));

	szString[0] = '_';
	szString[1] = '_';

	for(int i = 2; i < 10; i++)
	{
		szString[i] = char('a' + (rand() % 26));
	}

	szString[i] = 0;
}