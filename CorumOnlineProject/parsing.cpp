#include "stdafx.h"
#include "parsing.h"
#include "SafeMemory.h"
#include "StringFunc.h"


char g_szToken[0xff];

// ---------------------------------------------
// Name : GetLengthOfString
// Desc : 로컬 스트링을 얻어오는 함수.
//		  각국마다 문자 바이너리 정렬이 틀리다.
//		  그리하여 이 함수를 만들었다.
// parameters : 
//			iCount	- 얻어올 문자수 
//	[OUT]	pszDest - 얻은 문자열을 받아올 변수
//  [IN]	pszSrc	- 얻어올 문자열의 원본   
//								[6/22/2004] / by seven 
// ---------------------------------------------
bool GetLengthOfString(const char * IN pszSrc, char * OUT pszDest, int iCount)
{
	//////////////////////////////////////////////////////////////////////////
	// check valid of variable
	assert(pszDest && pszSrc);

	if (0 == iCount || iCount > (int)lstrlen(pszSrc))
	{
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
		
	SAFEMEMSET(pszDest, 0, sizeof(pszDest));	
	
	// iCount 개수 만큼 문자를 얻는다.
	char			szOneChar[5] = {0, };		// byte per character
	char *			pszNextChar;
	char *			pszPrevChar;
	int				iByteLen = 0;
	pszNextChar = (char *)pszPrevChar = (char *)pszSrc;

	for (int i = 0; i < iCount; i++)
	{	
		// 다음 문자로 이동 
		pszNextChar = CharNextExA(0, pszNextChar, 0);

		// 한 문자 복사한다.(Copy the one of character)
		iByteLen = (int)(pszNextChar - pszPrevChar);
		lstrcpyn(szOneChar, pszPrevChar, iByteLen+1);	// auto terminal null character, because do + 1
		lstrcat(pszDest, szOneChar);
		
		// 이전문자 저장 
		pszPrevChar = pszNextChar;									
	}

	return TRUE;
}


// ---------------------------------------------
// Name		: FindBeforeTokenStrings
// Desc		: 스트링 내용중 토큰이 존재하면 그이전 문자열까지 
//			  돌려주는 함수이이다.
//			  리턴되는 문자열은 글로벌로 변수로 잡아놓은 문자열 저장 공간이다.
// Aim		: 일단 속도보단 정확성과 재사용성을 염두에 둔다.
// Progress : 현재는 정해지지 않은 패턴이지만 토큰의 패턴을 정해주고 한다면
//			  토큰 시작 문자 끝문자를 두어 오버헤드를 줄일수 있다.
// Date / Writer :  [7/5/2004] / Seven
// ---------------------------------------------
char * 
FindBeforeTokenStrings(char * IN pszSrc, 
				 char ** IN ppszSeps, 
				 int iTokenStringCount)
{
	assert(iTokenStringCount > 0);
	assert(pszSrc && ppszSeps && *ppszSeps);

	int i, j;
	int iRet = 0;
	int iTokenLen = 0;
	int iSrcLen	= lstrlen(pszSrc);
//	int iTokenPos = 0;

	if (iSrcLen == 0)	return NULL;
	
	for (i = 0; i < iSrcLen; i++)
	{
		for (j = 0; j < iTokenStringCount; j++)
		{
			iTokenLen = lstrlen(ppszSeps[j]);

			if ( int(lstrlen(pszSrc+i) - iTokenLen) < 0 )
				continue;

			iRet = CompareString(LOCALE_USER_DEFAULT, NORM_IGNORECASE, pszSrc+i, iTokenLen, ppszSeps[j], iTokenLen );
			if (CSTR_EQUAL  == iRet)
			{
				__lstrcpyn(g_szToken, pszSrc, i);
				g_szToken[i] = 0;
				return (char *)(g_szToken);				
			}
		}
	}
	
	
	// 마지막 존재하는 문자열을 넘긴다.
	// 일단 여기까지 왔다는것은
	// 더이상 남아 있는 토큰은 없다는 뜻이다.
	__lstrcpyn(g_szToken, pszSrc, iSrcLen);
	g_szToken[i] = 0;
	return (char *)(g_szToken);
}




// ---------------------------------------------
// Name : OneCharPerStrLen
// Desc : 해외 / 국내 공용 
//		  순수히 인지가능한 문자개수를 얻어온다.
//		  문자열(문자)의 개수
// ---------------------------------------------
int OneCharPerStrLen(const char * IN pszSrc)
{
	assert(pszSrc);

	char * pszPos = (char *)pszSrc;

	int i = 0;
	for(;;)
	{	
		pszPos= CharNextExA(0, pszPos, 0);
		i++;
		if ( NULL == *pszPos ) break;
	}

	return i;
}

// ---------------------------------------------
// Name : IsThereString
// Desc : 해외 / 국내 공용 
//		  pszSrc 문자열에 어떤 pszDest 이 존재하는가?
// 
// parameters : 
//	[OUT]	pszSrc - 얻은 문자열을 받아올 변수
//  [IN]	pszDest	- 얻어올 문자열의 원본   
// date / writer :  [10/18/2004] / seven
// ---------------------------------------------
BOOL IsThereString(const char * IN pszSrc, const char * IN pszDest) 
{
	if (NULL == pszSrc && NULL == pszDest)
	{
		return FALSE;
	}

	int iSrcStringLenth = OneCharPerStrLen(pszSrc); 
	int iDestStringLenth = OneCharPerStrLen(pszDest); 

	if (iSrcStringLenth < iDestStringLenth)
	{
		return FALSE;
	}

	char *		pszSrcTempPos = (char *)&pszSrc[0];
//	int			iTokenLen = 0;
	int			iRet = 0;
	int			iTokenDestLen = lstrlen(pszDest);
	for (int i = 0; i < int(iSrcStringLenth - iDestStringLenth); i++)
	{
		iRet = CompareString(LOCALE_USER_DEFAULT, NULL, pszSrcTempPos, iTokenDestLen, pszDest, iTokenDestLen );
		if (CSTR_EQUAL  == iRet)
		{
			return TRUE;
		}

		pszSrcTempPos = CharNextExA(0, pszSrcTempPos, 0);
		if ( NULL == pszSrcTempPos ) break;
	}

	return FALSE;
}


// ---------------------------------------------
// Name : GetToken
// Desc : 1. 토큰 문자를 얻고 
//		  2. 토큰 문자 얻은 곳 다음 위치를 리턴해준다. 
//
// parameters :
//   [IN]	pszSrc		: 원본 스트링
//	 [OUT]	pszGetToken	: 토큰을 받아올 포인터 변수 
//			cToken		: 토큰 문자 
//
// return :
//		토큰 값을 얻고 토큰 다음 포인터를 리턴한다.
//
// date / writer: [10/29/2004] / seven 
// ---------------------------------------------
char * GetToken(char * IN pszSrc, char * OUT pszGetToken, char cToken /* = ' ' */)
{
	assert(pszGetToken);

	if (!pszSrc)	return NULL;

	int iLen = OneCharPerStrLen(pszSrc);
	if (0 == iLen)
	{
		// 여기 들어온 이상 더이상 토큰 얻을 필요가 없다.
		return NULL;
	}
	else 
	if (1 == iLen)
	{
		// 문자열이 하나이면 더이상 볼 필요 없다. 
		__lstrcpyn(pszGetToken, pszSrc, iLen);
		return NULL;
	}

	// 최소 문자열의 길이가 2 이상이다.

	int		iTokenCount = 0;
	for (char *	pszTempPos = pszSrc; NULL != *pszTempPos; 
		 pszTempPos = CharNextExA(0, pszTempPos, 0))
	{
		if (*pszTempPos == cToken)
		{
			iTokenCount++;
			pszTempPos = CharNextExA(0, pszTempPos, 0);		// 마지막 토큰은 스킵 
			break;
		}
	}

	int iTokenLen = (pszTempPos-pszSrc) - iTokenCount;

	// 1. 토큰까지의 문자를 얻는다. 

	__lstrcpyn(pszGetToken, pszSrc, iTokenLen);

	// 2. 토큰의 마지막 포인터로 시작 포인터를 리턴 
	
	return (NULL == *pszTempPos)? NULL : pszTempPos;		
}
