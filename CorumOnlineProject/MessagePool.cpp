#include "stdafx.h"
#include "MessagePool.h"

CMessagePool::CMessagePool()
{
	m_dwTextResourceCount	= 0; 
	m_pTextResourceInfo		= NULL;
	m_pTextResourceString	= NULL;
}

CMessagePool::~CMessagePool()
{
	ClearPool();
}

BOOL CMessagePool::AllocResourceInfo(DWORD dwCount, LPVOID pData)
{
	m_pTextResourceInfo = new TEXT_RESOURCE_INFO[dwCount];

#ifdef DEVELOP_MODE
	if(m_pTextResourceInfo == NULL) _asm int 3;
#endif

	if(m_pTextResourceInfo == NULL) return FALSE;

	if(pData)
	{
		memcpy(m_pTextResourceInfo, pData, dwCount * sizeof(TEXT_RESOURCE_INFO));
	}

	return TRUE;
}

BOOL CMessagePool::AllocResourceStringBuffer(DWORD dwSize, LPVOID pData)
{
	m_pTextResourceString = new BYTE [dwSize];

#ifdef DEVELOP_MODE
	if(m_pTextResourceString == NULL) _asm int 3;
#endif

	if(m_pTextResourceString == NULL) return FALSE;

	if(pData)
	{
		memcpy(m_pTextResourceString, pData, dwSize);
	}

	return TRUE;
}

MESSAGEx CMessagePool::operator [] (DWORD dwIndex)
{
	MESSAGEx Result = {0,};

#ifdef DEVELOP_MODE
	if(dwIndex >= m_dwTextResourceCount)	_asm int 3;
	if(m_pTextResourceString == NULL)		_asm int 3;
	if(m_pTextResourceInfo == NULL)			_asm int 3;
#endif

	LPSTR szText = LPSTR(m_pTextResourceString + m_pTextResourceInfo[dwIndex].dwTextPos);

	Result.dwId			= dwIndex;
	Result.szMessage	= szText;
	
	return Result;
}

LPSTR CMessagePool::GetMessage(DWORD dwIndex)
{
#ifdef DEVELOP_MODE
	if(dwIndex >= m_dwTextResourceCount)	_asm int 3;
	if(m_pTextResourceString == NULL)		_asm int 3;
	if(m_pTextResourceInfo == NULL)			_asm int 3;
#endif

	return LPSTR(m_pTextResourceString + m_pTextResourceInfo[dwIndex].dwTextPos);
}

BOOL CMessagePool::VerifySourceFile(LPTEXT_RESOURCE_HEADER pHeader, DWORD dwFileSize)
{
	if(pHeader->szSign[0] != 'O') return FALSE;
	if(pHeader->szSign[1] != 'o') return FALSE;
	if(pHeader->szSign[2] != 'p') return FALSE;
	if(pHeader->szSign[3] != 's') return FALSE;

	DWORD dwSize =	sizeof(TEXT_RESOURCE_HEADER) + 
					sizeof(TEXT_RESOURCE_INFO) * pHeader->dwResourceCount +
					pHeader->dwTextArraySize;

	return (dwFileSize == dwSize);
}

void CMessagePool::ClearPool()
{
	m_dwTextResourceCount = 0;

	if(m_pTextResourceString)
	{	
		delete [] m_pTextResourceString; m_pTextResourceString = NULL;
	}

	if(m_pTextResourceInfo)
	{	
		delete [] m_pTextResourceInfo; m_pTextResourceInfo = NULL;
	}
}

DWORD CMessagePool::GetFirstMessageIdInString(LPCSTR szString)
{
	DWORD dwMessageID = 0xffffffff;

	for(DWORD i = 0; i < GetCount(); i++)
	{
		// check filter word size!
		if(lstrlen(GetMessage(i)) == 0) continue;

		// check filter word!
		if(CompareRoutine(szString, GetMessage(i))) 
		{
			dwMessageID = i;
			break;
		}
	}
	
	return dwMessageID;	
}

BOOL CMessagePool::CompareRoutine(LPCSTR szString, LPCSTR szConv)
{
	LPSTR szTemp = (LPSTR)szString;

	for(;;)
	{
		if(lstrlen(szTemp) == 0) break;

		if(szTemp[0] != ' ')
		{
			int iRet = CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, 
				szConv, lstrlen(szConv), 
				szTemp, lstrlen(szConv));

			if(CSTR_EQUAL == iRet) return TRUE;
		}

		szTemp = CharNextExA(0, szTemp, 0);
	}	

	return FALSE;
}

int CMessagePool::CompareRoutineEmoticon(LPCSTR szString)
{
	for(int i = 0; i < (int)GetCount(); i++)
	{
		// check filter word size!
		if(lstrlen(GetMessage(i)) == 0) continue;
		if(lstrlen(GetMessage(i)) > lstrlen(szString)) continue;

		int iRet = CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, 
			szString, lstrlen(GetMessage(i)), 
			GetMessage(i),
			lstrlen(GetMessage(i)));

		if(CSTR_EQUAL == iRet) return i;
	}
	
	return -1;	
}

int CMessagePool::FilterEmoticonString(LPCSTR szString)
{
	if(lstrlen(szString) == 0) return -1;

	int iMessageID = -1;
	LPSTR szTemp = LPSTR(szString+(lstrlen(szString)-1));
	
	for(;;)
	{
		szTemp = CharPrevExA(0, szString, szTemp, 0);

		iMessageID = CompareRoutineEmoticon(szTemp);
		if(iMessageID != -1) return iMessageID;
		
		if(szTemp == szString) break;
	}

	return iMessageID;
}

BOOL CMessagePool::IsThereAnyMessageInString(LPCSTR szString)
{
	return (0xffffffff != GetFirstMessageIdInString(szString));
}

LPCSTR CMessagePool::GetFirstMessageInString(LPCSTR szString)
{
	DWORD dwIndex = GetFirstMessageIdInString(szString);
	return (0xffffffff == dwIndex) ? NULL : LPCSTR(GetMessage(dwIndex));
}

BOOL CMessagePool::LoadTextResource(char* szSourceFile, char* szDecodeKey, int nDecodeSubKey)
{
	LPBYTE pBuf = NULL;
	BOOL bRet = FALSE;
	FILE* fp = NULL;
	DWORD dwFileSize = 0;
	LPBYTE pPos = NULL;
	LPTEXT_RESOURCE_HEADER pHeader = NULL;

	__try
	{
		ClearPool();
		
		fp = fopen(szSourceFile, "rb");
		if(fp == NULL) __leave;		
		
		fread(&dwFileSize, sizeof(DWORD), 1, fp);

		pBuf = new BYTE [dwFileSize];
		if(NULL == pBuf) __leave;

		// data converstion starts.
		if(!DecodeData4Text(szSourceFile, pBuf, szDecodeKey, nDecodeSubKey)) __leave;
		// data conversion ends.

		pPos = pBuf;

		pPos += sizeof(DWORD); 

		pHeader = (LPTEXT_RESOURCE_HEADER)pPos; 
		pPos += sizeof(TEXT_RESOURCE_HEADER);

		// verify!!
		if(!VerifySourceFile(pHeader, dwFileSize-sizeof(DWORD))) __leave;

		// set pool data!!
		m_dwTextResourceCount = pHeader->dwResourceCount;

		if(!AllocResourceInfo(pHeader->dwResourceCount, pPos)) __leave;
		pPos += sizeof(TEXT_RESOURCE_INFO)*pHeader->dwResourceCount;

		if(!AllocResourceStringBuffer(pHeader->dwTextArraySize, pPos)) __leave;

		bRet = TRUE;		
	}
	__finally
	{
		if(fp) fclose(fp);
		if(pBuf) delete [] pBuf;
		if(!bRet) ClearPool();

		return bRet;
	}
}

BOOL CMessagePool::DecodeData4Text(char* szLoadFile,  void* pReturnValue, char* szDecodeKey, int nDecodeSubKey)
{
	FILE*	fp = NULL;	
	DWORD	dwCur = 0;
	DWORD   dwTotalLen = 0;
	BOOL	bRet = FALSE;
	BOOL	bContinue = TRUE;
	char*	szBuffer = (char*)pReturnValue;
	int		nKeyLen = 0;

	__try
	{		
		nKeyLen = lstrlen(szDecodeKey);
		fp = fopen( szLoadFile, "rb" );
		if(!fp) __leave;

		fread(&dwTotalLen, sizeof(DWORD), 1, fp );

		while( bContinue )
		{
			if(!fread(szBuffer + dwCur, nKeyLen, 1, fp ))
				bContinue = FALSE;

			if(bContinue)
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
		
		bRet = TRUE;
	}

	__finally
	{
		if(fp) fclose(fp);

		return bRet;
	}
}
