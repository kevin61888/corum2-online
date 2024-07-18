// TokenCommand.cpp: implementation of the CTokenCommand class.
//
//////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "StdAfx.h"
#include "TokenCommand.h"
#include "Parsing.h"
#include "NetworkClient.h"
#include "GameControl.h"
#include "DUNGEON_DATA_EX.h"
#include "user.h"


BEGIN_COMMAND_TOKEN_ENTRY(CTokenCommand)
	ON_GM_COMMAND(GM_Command_Create_Item, 			"create item")
	ON_GM_COMMAND(GM_Command_Summon_Monster,		"summon monster")
	ON_GM_COMMAND(GM_Command_Silence,				"silence")
	ON_GM_COMMAND(GM_Command_Get_Recall_Someone,	"get recall")
	ON_GM_COMMAND(GM_Command_Get_Move_Someone,		"get move")
	ON_GM_COMMAND(GM_Command_Dungeon_Portal,		"dungeon potal")
	ON_GM_COMMAND(GM_Command_PK_Mode,				"pk mode")
	ON_GM_COMMAND(GM_Command_Observe,				"observe")
	ON_GM_COMMAND(GM_Command_Join_Guild,			"join guild")
	ON_GM_COMMAND(GM_Command_Change_Monster,		"change monster")
	ON_GM_COMMAND(GM_Command_Kick_User,				"kick")
END_COMMAND_TOKEN_ENTRY()


CTokenCommand * CTokenCommand::m_pTokenCommand = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTokenCommand::CTokenCommand()
{
	Initialize();
}

CTokenCommand::~CTokenCommand()
{
	if (m_pTokenCommand)
	{
		delete m_pTokenCommand;
		m_pTokenCommand = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////


CTokenCommand *	CTokenCommand::GetInstance()
{
	if (!m_pTokenCommand)
		m_pTokenCommand	= (CTokenCommand *)new CTokenCommand;

	return (CTokenCommand *)m_pTokenCommand;
}


void CTokenCommand::Initialize() 
{
	m_sCurrentTokenEntry.iMsgType	= MSG_Type_Empty;
	m_sCurrentTokenEntry.iCommandID = GM_Command_Empty;
	m_bCorrectGrammarCommand = FALSE;
}

BOOL CTokenCommand::Execute()
{
	if (!IsValidCommand())
	{
		return FALSE;
	}

	switch (GetCommandMsgType())
	{
	case MSG_Type_GM:
		SendPacketGMCommand();
		return TRUE;
	break;


	case MSG_Type_General:		
		{
			_asm nop // 아직 구현 되지 않음
		}
	default:
		wsprintf(m_szTempString, "Invalid Command Msg [ type : %d, Command ID :%d! ]", 
			m_sCurrentTokenEntry.iMsgType, m_sCurrentTokenEntry.iCommandID);
		DEBUG_LOG(m_szTempString);
		return FALSE;
	} 

	// flag 들을 다시 초기화 해 놓는다.
	m_bCorrectGrammarCommand = FALSE;
	m_bIsCommand			 = FALSE;

	return TRUE;
}

void CTokenCommand::InputString(char * pszString)
{
	assert(pszString);
	
	int iLen = lstrlen(pszString);
	if (!iLen)	return;

	if (iLen > MAX_TOKEN_LENGTH )	return;
	
	const int iSize = iLen-1;

	if ('/' == pszString[0] 
		&&	0 < iSize) 
	{ 
		m_bIsCommand = TRUE;
	
		__lstrcpyn( m_szTempString, pszString+1, iSize ); 
		m_sCurrentTokenEntry = WhatKindOfGMMessage( m_szTempString );
		ParseGMCommand();
	}
	else
	{
		m_bIsCommand = FALSE;
	}

}



COMMAND_TOKEN_ENTRY CTokenCommand::WhatKindOfGMMessage(char * pszString)
{
	int iLen = lstrlen(pszString);
	
	for (int i = 0; 0 != _TokenCommandEntries[i].iMsgType; i++)
	{
		if (iLen < lstrlen(_TokenCommandEntries[i].pszToken))
			continue;

		if (0 == __strnicmp(_TokenCommandEntries[i].pszToken, pszString, 
								lstrlen(_TokenCommandEntries[i].pszToken)) )
		{
			// 토큰과 일치하는 스트링이다.
			break;
		}
	}

	return _TokenCommandEntries[i];
}


BOOL CTokenCommand::IsCommand()	const
{
	return BOOL(m_bIsCommand);
}
	

//////////////////////////////////////////////////////////////////////////
// private methods
int CTokenCommand::GetCommandMsgType() const
{
	return m_sCurrentTokenEntry.iMsgType;
}

int CTokenCommand::Get_GM_MsgType() const
{
	return m_sCurrentTokenEntry.iCommandID;
}

int CTokenCommand::ParseGMCommand()
{
	int iStartParameterPos = lstrlen(m_sCurrentTokenEntry.pszToken)+1;
	char szGetToken[0xff];
	char *	pszParam = m_szTempString;

	m_bCorrectGrammarCommand = FALSE;

	m_sDungeonGMCommand.sBaseGMCmdParam.eGMCommand = 
		m_sWorldGMCommand.sBaseGMCmdParam.eGMCommand = (GM_COMMAND)Get_GM_MsgType();
		

	switch (m_sDungeonGMCommand.sBaseGMCmdParam.eGMCommand)
	{
	case GM_Command_Create_Item:
		{
			//
			// Item 생성 
			//

			// item id
			// 아이템 생성 개수는 현재 1개로 픽스 되어있다.
			// 그래서 id 만 얻어온다.	

			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);			
			int iItemID = atoi(szGetToken);

			m_sDungeonGMCommand.sGMCmdParamCreateItem.wItemID = (WORD)iItemID;
			

			goto Parsing_Sucess;
			
			break;
		} 

	case GM_Command_Summon_Monster:	
		{
			//
			// 몬스터 불러옴 
			//


			// Monster ID
			// [Monster Count]
			// [소환할 몬스터 id* 소환수량(min 5 ~ max 50)를 입력]

			//
			// Get Monster ID
			//
			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);		
			if (!szGetToken)
			{
				goto Parsing_Fail;
			}

			m_sDungeonGMCommand.sGMCmdParamSummonMonster.wMonsterID = (WORD)atoi(szGetToken);		
			m_bCorrectGrammarCommand = TRUE;

			m_sDungeonGMCommand.sGMCmdParamSummonMonster.wMonsterCount = SUMMON_MONSTER_MIN_COUNT;
			//
			// Get Monster Count 
			//
			if (!pszParam)
			{
				goto Parsing_Sucess;
			}
			pszParam = GetToken(pszParam, szGetToken);		
			m_sDungeonGMCommand.sGMCmdParamSummonMonster.wMonsterCount = 
							(WORD)min(atoi(szGetToken), SUMMON_MONSTER_MAX_COUNT);	// SUMMON_MONSTER_MAX_COUNT 이하 

			goto Parsing_Sucess;
		}
		break;

	case GM_Command_Silence:
		{
			
			// ON or OFF
			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);		
			if (!__stricmp(szGetToken, "on"))
			{
				// 챗 금지 
				m_sDungeonGMCommand.sGMCmdParamSilence.bFlag = TRUE;		// 침묵 		

			}
			else 
			if (!__stricmp(szGetToken, "off"))
			{
				// 챗 가능 
				m_sDungeonGMCommand.sGMCmdParamSilence.bFlag = FALSE;
			}
			else
			{
				goto Parsing_Fail;
			}
	
			// ALL or character name
			pszParam = GetToken(pszParam, szGetToken);		
			if (!__stricmp(szGetToken, "all"))
			{
				// 해당던전 내의 모든 유저 적용 
				m_sDungeonGMCommand.sGMCmdParamSilence.bAll = TRUE;
			}
			else
			{
				// 해당 캐릭터만 침묵 
				m_sDungeonGMCommand.sGMCmdParamSilence.bAll = FALSE;		

				DWORD dwSilenceUserIndex = GetUserIndexByNameFromUserHash(szGetToken);
				if (0 == dwSilenceUserIndex)
				{
					goto Parsing_Fail;							
				}

				m_sDungeonGMCommand.sGMCmdParamSilence.dwUserIndex = dwSilenceUserIndex;			
			}

			goto Parsing_Sucess;
		}
		break;
	case GM_Command_Get_Recall_Someone:
		{
			// 케릭터 이름  
			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);
			if (!__stricmp(szGetToken, ""))
			{
				// 아무것도 입력하지 않았다!
				goto Parsing_Fail;
			}

			__lstrcpyn(m_sWorldGMCommand.sGMCmdParamRecallSomeone.szCharacterName, 
												szGetToken, lstrlen(szGetToken));


			// GM 이 있는 곳을 알려준다. . 
			m_sWorldGMCommand.sGMCmdParamRecallSomeone.wDungeonID = (WORD)g_pThisDungeon->m_dwID;
			m_sWorldGMCommand.sGMCmdParamRecallSomeone.bLayer	  = (BYTE)g_pMainPlayer->m_bCurLayer;
			m_sWorldGMCommand.sGMCmdParamRecallSomeone.wX		  = (WORD)g_pMainPlayer->m_v3CurPos.x;
			m_sWorldGMCommand.sGMCmdParamRecallSomeone.wZ		  = (WORD)g_pMainPlayer->m_v3CurPos.z;
			
			goto Parsing_Sucess;
		}

		break;
	case GM_Command_Get_Move_Someone:
		{
			// 케릭터 이름  
			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);
			if (!__stricmp(szGetToken, ""))
			{
				// 아무것도 입력하지 않았다!
				goto Parsing_Fail;
			}

			// szGetToken - 캐릭터 이름
			__lstrcpyn(m_sWorldGMCommand.sGMCmdParamGetMoveSomeone.szCharacterName, 
												szGetToken, lstrlen(szGetToken));
		}
		break;

	case GM_Command_Dungeon_Portal:
		{
			//
			// Dungeon ID
			//
			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);
			if (!__stricmp(szGetToken, ""))
			{
				// 아무것도 입력하지 않았다!
				goto Parsing_Fail;
			}

			m_sWorldGMCommand.sGMCmdParamDungeonPortal.wDungeonID = (WORD)atoi(szGetToken);

			//
			// Dungeon Layer (몇층)
			//
			pszParam = GetToken(pszParam, szGetToken);

			if (!__stricmp(pszParam, ""))
			{
				// 아무것도 입력하지 않았다!
				// default 0 
				m_sWorldGMCommand.sGMCmdParamDungeonPortal.bLayerFloor = 0;

				goto Parsing_Sucess;
			}

			m_sWorldGMCommand.sGMCmdParamDungeonPortal.bLayerFloor = (BYTE)atoi(szGetToken);
			goto Parsing_Sucess;
		}
		break;

	case GM_Command_PK_Mode:
		{
			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);
			if (!__stricmp(szGetToken, ""))
			{
				// 아무것도 입력하지 않았다!
				goto Parsing_Fail;
			}		

			if (!__stricmp(szGetToken, "on"))
			{
				m_sDungeonGMCommand.sGMCmdParamPKMode.bPKMode = TRUE;
			}
			else
			if (!__stricmp(szGetToken, "off"))
			{
				m_sDungeonGMCommand.sGMCmdParamPKMode.bPKMode = FALSE;
			}
			else
			{
				goto Parsing_Fail;
			}

			goto Parsing_Sucess;
		}
		break;

	case GM_Command_Join_Guild:
		{
			// terms
			// gm케릭터가 observe상태로 특정 길드에 가입됨. 
			// 로그아웃하면 자동탈퇴
			//
			// Not yet Implement 
			
		}
		break;
	case GM_Command_Change_Monster:	 
		{
			// terms
			// 몬스터 변신 명령어. 
			// gm케릭터가 특정 몬스터로 변신함.
			// 로그아웃하면 자동해제	
			// Not yet Implement
		}
		break;
	case GM_Command_Kick_User:
		{
			// 특정 유저를 킥한다.
			// 케릭터 이름  

			if( !g_pMainPlayer->IsGMUser() )//GM이 아니면
			{
				goto Parsing_Fail;
			}

			pszParam = GetToken(pszParam+iStartParameterPos, szGetToken);
			if (!__stricmp(szGetToken, ""))
			{
				// 아무것도 입력하지 않았다!
				goto Parsing_Fail;
			}

			// szGetToken - 캐릭터 이름
			__lstrcpyn(m_sWorldGMCommand.sGMCmdParamKickUser.szCharacterName, 
												szGetToken, lstrlen(szGetToken));
			goto Parsing_Sucess;
		}
		break;
	
	case GM_Command_Empty:
	default:
		DEBUG_LOG("Not yet process Command!!");
		m_bCorrectGrammarCommand = FALSE;

		return m_sDungeonGMCommand.sBaseGMCmdParam.eGMCommand;
		break;
	}



Parsing_Sucess:
	m_bCorrectGrammarCommand = TRUE;
	return TRUE;

Parsing_Fail:
	m_bCorrectGrammarCommand = FALSE;
	return FALSE;
}

void CTokenCommand::SendPacketGMCommand() 
{
	if (!IsValidCommand())
	{
		return;
	}

	switch (Get_GM_MsgType())
	{
	case GM_Command_Create_Item:
	case GM_Command_Summon_Monster:	
	case GM_Command_Silence:
	case GM_Command_Change_Monster:	 
	case GM_Command_PK_Mode:
		g_pNet->SendMsg((char *)&m_sDungeonGMCommand, 
			m_sDungeonGMCommand.GetPacketSize(), 
			SERVER_INDEX_ZONE);
		break;

	case GM_Command_Get_Recall_Someone:
	case GM_Command_Get_Move_Someone:
	case GM_Command_Dungeon_Portal:
	case GM_Command_Join_Guild:
	case GM_Command_Kick_User:
		g_pNet->SendMsg((char *)&m_sWorldGMCommand,
			m_sWorldGMCommand.GetPacketSize(),
			SERVER_INDEX_WORLD);
		break;

	case GM_Command_Empty:
	default:
		break;
	}


}


