#include "stdafx.h"
#include "Filter.h"


FILTER_WORDS			g_FilterWords[MAX_FILTER_WORDS];
DWORD					g_FilterWordCount = 0;
extern IOleDBThread*	g_pDb;


//////////////////////////////////////////////////////////////////////////////////////
static BOOL CompareRoutine(LPSTR szString, LPSTR szConv)
{
	LPSTR szTemp = (LPSTR)szString;

	for(;;)
	{
		if(lstrlen(szTemp) == 0) break;

		if(szTemp[0] != ' ')
		{
			if(0 == __strnicmp(szConv, szTemp, lstrlen(szConv)))
				 return TRUE;
		}

		szTemp = CharNextExA(0, szTemp, 0);
	}	

	return FALSE;
}

void InitLimitedWords()
{
	DBRECEIVEDATA*		pData = 0;
	DWORD i=0;
	DWORD dwIndex = 0;	

	Log(LOG_JUST_DISPLAY, "@ Loading Limited Words...");

	ZeroMemory(g_FilterWords, sizeof(FILTER_WORDS)*512);

	pData = g_pDb->OpenRecordEx("select * from filter_conv_message", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(i=0; i<pData->Query.select.dwRowCount; i++)
	{
		memcpy( &g_FilterWords[dwIndex++], pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
	}
	
	g_FilterWordCount += pData->Query.select.dwRowCount;

	g_pDb->ReleaseRecordset(pData);	
	
	pData = g_pDb->OpenRecordEx("select * from filter_notconv_message", 500, DEFAULT_ROWS_PER_READ, (BYTE)GAME_DB );
	for(i=0; i<pData->Query.select.dwRowCount; i++)
	{
		memcpy( &g_FilterWords[dwIndex++], pData->Query.select.pResult + ( i * pData->Query.select.dwRowSize ), pData->Query.select.dwRowSize );
	}
	
	g_FilterWordCount += pData->Query.select.dwRowCount;

	g_pDb->ReleaseRecordset(pData);

	Log(LOG_JUST_DISPLAY, "@ Limited Words Loaded ...");
}

BOOL FilterWordsIncluded(LPSTR szString)
{
	for(DWORD i = 0; i < g_FilterWordCount; i++)
	{
		// check filter word size!
		if(lstrlen(g_FilterWords[i].szFilter) == 0) continue;

		// check filter word!
		if(CompareRoutine(szString, g_FilterWords[i].szFilter)) 
			return TRUE;
	}
	
	return FALSE;	
} 

