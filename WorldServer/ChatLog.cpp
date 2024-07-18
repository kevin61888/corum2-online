#include "stdafx.h"
#include "ChatLog.h"
#include "WorldUser.h"
#include "OwnServer.h"


BOOL SetChatLogInWorld(CHAT_TYPE eChatType, CWorldUser * pSendUser, CWorldUser * pRecvUser, char * pszChat, int ChatLlen)
{	
	if (!g_bEnableChatLog)	
	{
		return FALSE;
	}

	if (NULL == pSendUser)	
	{
		return FALSE;
	}

	if (pszChat == NULL)
	{
		return FALSE;
	}

	CHAT_LOG	ChatLog;
	
	//DB Updates..
	char		szQuery[1024]={0,};
	WORD			nColumn			= 6;	
	int			col				= 0;
	DWORD		dwParamOffset	= 0;
	DBBINDING*	pBinding		= g_pLogDb->THCreateParamInfo(nColumn);	

	if(!pBinding)
	{
		Log(LOG_IMPORTANT, "ParameterPool is Full!!  in SetItemLog Function()!");
		return 0;
	}
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(ChatLog.bMessageType);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(ChatLog.bMessageType);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof( ChatLog.bUserType);
	pBinding[col++].wType	= DBTYPE_UI1;		
	dwParamOffset			+= sizeof(ChatLog.bUserType);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof( ChatLog.szSender);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(ChatLog.szSender);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof( ChatLog.szRecver);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(ChatLog.szRecver);

	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(ChatLog.wDungeonID);
	pBinding[col++].wType	= DBTYPE_I2;		
	dwParamOffset			+= sizeof(ChatLog.wDungeonID);
	
	pBinding[col].obValue	= dwParamOffset;
	pBinding[col].cbMaxLen	= sizeof(ChatLog.szContents);
	pBinding[col++].wType	= DBTYPE_STR;		
	dwParamOffset			+= sizeof(ChatLog.szContents);

	SYSTEMTIME CurTime;
	GetLocalTime(&CurTime);
	wsprintf( 
		szQuery, 
		"Insert into Chat_Log_%d_%d%02d%02d (MessageType, UserType, Sender, Recver, DungeonID,  Contents) values( ?, ?, ?, ?, ?, ? )",
		BYTE(atoi(g_pThis->GetServerSetCode())), 
		CurTime.wYear, CurTime.wMonth, CurTime.wDay
		);

	// 로그에 남길내용을 넣어준다. 
	ChatLog.bMessageType	= (BYTE)eChatType;
	ChatLog.bUserType		= (BYTE)pSendUser->m_bUserType; 
	ChatLog.wDungeonID		= (WORD)pSendUser->m_dwDungeonID;
	__lstrcpyn(ChatLog.szContents, pszChat, ChatLlen);
	__lstrcpyn(ChatLog.szSender, pSendUser->m_szCharacterName, lstrlen(pSendUser->m_szCharacterName));

	switch(eChatType) 
	{
	case Chat_Type_GUILD:
		__lstrcpyn(ChatLog.szRecver, pRecvUser->m_szGuildName, lstrlen(pRecvUser->m_szGuildName));
		break;
	case Chat_Type_ALLCHAT:
	case Chat_Type_WHISPER:
	case Chat_Type_PARTY:
	case Chat_Type_FRIEND:
	default:
		__lstrcpyn(ChatLog.szRecver, pRecvUser->m_szCharacterName, lstrlen(pRecvUser->m_szCharacterName));
	} 

	
	g_pLogDb->THExecuteSQLByParam( szQuery, 
		pBinding, 
		&ChatLog.bMessageType, 
		sizeof(ChatLog), 
		(BYTE)nColumn, 
		FALSE, 
		0, 
		NULL, 
		(BYTE)LOG_DB
		);
	
	return TRUE;	
}

