
#include "stdafx.h"
#include "Filter.h"
#include "InitGame.h"
#include "chat.h"
#include "message.h"

// --------------------------------------------------- 
// Changed Newly by deepdark for multilangual support.
// ---------------------------------------------------


BOOL ConvString(char* szStr)
{
	return g_ConvMessage.IsThereAnyMessageInString(szStr);
}

BOOL NoConvString(char* szStr)
{
	return g_NotConvMessage.IsThereAnyMessageInString(szStr);
}

int EmoticonString(char* szStr)
{
	int iIndex = g_Emoticon.CompareRoutineEmoticon(szStr);

	// +1 하는 이유는 기존 이모티콘이 모두 1부터 시작했으나 지금은 0부터 시작함.
	return (-1 == iIndex) ? -1 : ((iIndex % MAX_EMOTICON)+1);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CheckString(LPSTR szString, BOOL bCheckLimited)
{
	int nRt = ConvString(szString);

	if(nRt==1)
	{																		
		DisplayMessageAdd(g_Message[ETC_MESSAGE58].szMessage, 0xFFFF0000);	// MSG_ID : 58 ; 비속어나 욕설은 사용할 수 없습니다.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);

		return TRUE;
	}

	if(!bCheckLimited) return FALSE;

	nRt = NoConvString(szString);

	if(nRt==1)
	{				
		DisplayMessageAdd(g_Message[ETC_MESSAGE59].szMessage, 0xFFFF0000);		// MSG_ID : 59 ; 금지 단어입니다. 사용할 수 없습니다.
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		
		return TRUE;									
	}

	return FALSE;
}