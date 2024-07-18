#include "StdAfx.h"
#include "CharSelectMsg.h"
#include "CodeFun.h"
#include "CharSelectProcess.h"
#include "InitGame.h"
#include "GameControl.h"
#include "CorumResource.h"
#include "DefResource.h"
#include "ObjectManager.h"
#include "User.h"
#include "BlockWnd.h"
#include "Struct.h"
#include "HashTable.h"
#include "GameDefaultWnd.h"
#include "message.h"
#include "ErrorWnd.h"
#include "GlobalSoundHandles.h"
#include "InputManager.h"
#include "IMEEdit.h"
#include "WorldWnd.h"
#include "QuantityWnd.h"
#include "NetworkClient.h"
#include "LoginProcess.h"
#include "CharMoveWnd.h"
#include "CharMoveRtWnd.h"
#include "CharMoveEditWnd.h"
#include "WorldProcess.h"
#include "CharSelectMsg.h"
#include "DungeonProcess.h"
#include "NetworkTimeObserver.h"


void CmdWorldUserInfo( char* pMsg, DWORD dwLen )
{
	WSTC_WORLD_USER_INFO* pPacket = (WSTC_WORLD_USER_INFO*)pMsg;	

	g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_START_SUCCESS;
	
#ifndef DEVELOP_MODE
	CreateMainPlayer( pPacket );
	SetGameStatus(UPDATE_GAME_WORLD);		//월드맵으로 이동 
#else

	if(g_Dev.bBeginType == DEVELOP_START_NORMAL)
	{
		CreateMainPlayer( pPacket );
		SetGameStatus(UPDATE_GAME_WORLD);		//월드맵으로 이동 

		g_pMainPlayer->m_byType		= pPacket->byType;
		g_pMainPlayer->m_byRank		= pPacket->byRank;
		g_pMainPlayer->m_dwGuildId	= pPacket->dwGuildId;
		g_pMainPlayer->m_dwGuildWarFlag = pPacket->dwGuildWarFlag;


		__lstrcpyn(g_pMainPlayer->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
		__lstrcpyn(g_pMainPlayer->m_szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
		
		BYTE byRHand = EQUIP_TYPE_RHAND1;

		if(g_pMainPlayer->m_bCurrnetHand==2)
			byRHand = EQUIP_TYPE_RHAND2;

		int nVal = pPacket->Equip[byRHand].m_wItemID/ITEM_DISTRIBUTE;	

		if(pPacket->Equip[byRHand].m_wItemID==0)
			g_pMainPlayer->m_byItemType	= 0;
		else
			g_pMainPlayer->m_byItemType	= nVal+1;

		g_pMainPlayer->SetAction(MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP);
		return;
	}
	
	if(g_Dev.bBeginType == DEVELOP_START_WORLD)
	{
		g_pExecutive->SetFramePerSec(25);
		g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(TRUE);	
		SetGameStatus(UPDATE_GAME_WORLD);
		InitGameWorld();
	}
	
	if(g_Dev.bBeginType == DEVELOP_START_MAP)
	{
		g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(TRUE);	
		LoadTotalDungeonInfo();	//DungeonTable 채우고 
		g_pGeometry->SetShadowFlag(ENABLE_PROJECTION_SHADOW |	ENABLE_PROJECTION_TEXMAP);
		g_pExecutive->SetFramePerSec(25);
		
		CTWS_TRY_TO_JOIN_DUNGEON packet;
		packet.wDungeonID = g_Dev.wBeginMapID;
		g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_AGENT);
	}
#endif

	g_pMainPlayer->m_byType		= pPacket->byType;
	g_pMainPlayer->m_byRank		= pPacket->byRank;
	g_pMainPlayer->m_dwGuildId	= pPacket->dwGuildId;
	g_pMainPlayer->m_dwGuildWarFlag = pPacket->dwGuildWarFlag;

	__lstrcpyn(g_pMainPlayer->m_szGuildName, pPacket->szGuildName, MAX_GUILD_NAME_REAL_LENGTH);
	__lstrcpyn(g_pMainPlayer->m_szNickName, pPacket->szNickName, MAX_NICK_REAL_NAME);
	
	BYTE byRHand = EQUIP_TYPE_RHAND1;

	if(g_pMainPlayer->m_bCurrnetHand==2)
		byRHand = EQUIP_TYPE_RHAND2;

	int nVal = pPacket->Equip[byRHand].m_wItemID/ITEM_DISTRIBUTE;	

	if(pPacket->Equip[byRHand].m_wItemID==0)
		g_pMainPlayer->m_byItemType = 0;
	else
		g_pMainPlayer->m_byItemType	= BYTE(nVal+1);

	g_pMainPlayer->SetAction(MOTION_TYPE_VILLAGESTAND, 0, ACTION_LOOP);	
}

void AcmdWarningTitle(char* pMsg, DWORD dwLen )
{
	WSTC_WARNING_TITLE*	pPacket = (WSTC_WARNING_TITLE*)pMsg;
	CBlockWnd*			pWnd	= CBlockWnd::GetInstance();
	
	pWnd->m_bInfoType = UPDATE_GAME_CHAR_SELECT;

	char*	szTemp = 0;
	char	szReason[1024]={0,};	
	int		nMaxLineCharacter = 34;

	memset(szReason, 0, 1024);
	memcpy(szReason, pPacket->szReason, pPacket->bLen);

	szTemp	= szReason;
	int nSize	= pPacket->bLen;
	int nCount = 1;
	
	for(int i=0; i<MAX_BLOCK_MESSAGE_LINE_COUNT; i++)
	{
		memset(pWnd->szMessageLine[i], 0, 255);
	}

	lstrcpy(pWnd->szMessageLine[0], g_Message[ETC_MESSAGE922].szMessage); // "<< 귀하는 현재 경고중인 계정입니다. >>"

	while(nSize > 0)
	{
		if(nSize > nMaxLineCharacter)
		{
			for(;;)
			{
				if(nSize > nMaxLineCharacter)
				{
					char* szPrev = CharPrevExA(0, szTemp, szTemp+nMaxLineCharacter, 0);
					nSize = lstrlen(szTemp) - lstrlen(szPrev);
					
					memset(pWnd->szMessageLine[nCount], 0, 255);
					__lstrcpyn(pWnd->szMessageLine[nCount], szTemp, nSize);
					
					szTemp = szPrev;
					nCount++;
					nSize = lstrlen(szTemp);
				}
				else
				{
					// 구한 문자열을 채팅 라인에 쓰고, 길이를 기록한다.
					lstrcpy(pWnd->szMessageLine[nCount], szTemp);
					nCount++;								
					nSize = 0;
					break;
				}
			}			
		}
		else
		{	
			memset(pWnd->szMessageLine[nCount], 0, nMaxLineCharacter);
			__lstrcpyn(pWnd->szMessageLine[nCount], szTemp, lstrlen(szTemp));	
			nCount++;
			nSize = 0;
		}		
	}
	pWnd->m_fPosX		= 402;
	pWnd->m_fPrevPosX	= pWnd->m_fPosX;
	pWnd->m_fPosZ		= 250;
	pWnd->m_fPrevPosZ	= pWnd->m_fPosZ;
	pWnd->SetActive();
}

void CmdChrMoveChk(char* pMsg, DWORD dwLen)
{
	ASTC_CHR_MOVECHK*	pPacket			= (ASTC_CHR_MOVECHK*)pMsg;
	CCharMoveWnd*		pCharMoveWnd	= CCharMoveWnd::GetInstance();
	CErrorWnd*			pErrorWnd		= CErrorWnd::GetInstance();
		
	char szInfo[0xff]	= {0, };
//	char szStr[0xff]	= {0,};

	g_byCharMoveCnt = 0;

	if(pPacket->byType==0xff)
	{
		// 이전할 캐릭터가 빈 슬롯 보다 많을 경우 //
		// "캐릭터 슬롯이 부족합니다."
		// "캐릭터를 삭제 후 이전해 주십시요."
		pErrorWnd->SetError(g_Message[ETC_MESSAGE1090].szMessage, g_Message[ETC_MESSAGE1091].szMessage, "", 2);
		pErrorWnd->SetActive(TRUE);
	}
	else
	{
		// "%d개의 캐릭터의 정보가 이전되었습니다."
		wsprintf(szInfo, g_Message[ETC_MESSAGE1110].szMessage, pPacket->byCnt);

		// "캐릭터를 이전 하시겠습니까?"
		pCharMoveWnd->SetText(szInfo, g_Message[ETC_MESSAGE1093].szMessage, "", 2); 
		pCharMoveWnd->SetActive();
		g_bCharMoveChk	= TRUE;
		g_byCharMoveCnt = pPacket->byCnt;
	}
}

void CmdMoveCharacterSuccess(char* pMsg, DWORD dwLen)
{
	ASTC_MOVE_CHARACTER_SUCCESS*	pPacket			= (ASTC_MOVE_CHARACTER_SUCCESS*)pMsg;
	CCharMoveRtWnd*					pCharMoveRtWnd	= CCharMoveRtWnd::GetInstance();

	if(IsEmptyString(pPacket->Character.szName)) return;
	
	char szInfo[0xff] = {0, };

	WORD wIndex = WORD(pPacket->byCharNum - 1);

	if(wIndex > 3 || wIndex > 256)
	{
		CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

		// MSG_ID : 322 ; Invalid Character Index Received!
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE322].szMessage, "", "", 402, 459, 1);	
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);
		pGameDefaultWnd->SetActive();
		LoginInterface(FALSE);			
		PostQuitMessage(0);

		if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
		{
			g_bIsRunning = FALSE;
			return;
		}
	}

	if(!IsEmptyString(g_pGVLogin->ChrSelectInfo[ wIndex ].ReceivedChrInfo.szName))
	{
		CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();

		// MSG_ID : 323 ; Character Overlapped!
		pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE323].szMessage, "", "", 402, 459, 1);	
		_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_ERRORMSG, g_v3InterfaceSoundPos, FALSE);				
		pGameDefaultWnd->SetActive();
		
		if(g_bExecuteType == EXECUTE_TYPE_NETMARBLE)
		{
			pGameDefaultWnd->m_bExit = TRUE;
			return;
		}
	}
	
	memcpy(&g_pGVLogin->ChrSelectInfo[ wIndex ], &pPacket->Character, sizeof(RS_CharacterLogInfo) );		
	g_pGVLogin->ChrSelectInfo[ wIndex ].bByItemType = BYTE((pPacket->Character.wRHandID/ITEM_DISTRIBUTE) + 1);

	if(IsEmptyString(g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.szName)) 
		return;

	g_pGVLogin->ChrSelectInfo[wIndex].pDesc = AllocObjDesc();
	memset(g_pGVLogin->ChrSelectInfo[wIndex].pDesc, 0, sizeof(OBJECT_DESC));	
	g_pGVLogin->ChrSelectInfo[wIndex].pDesc->bType = OBJECT_TYPE_SELCHAR;
	
	WORD wItemId[4] = {	g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wMailID,
						g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wRHandID,
						g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wLHandID,
						g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wHelmetID};
	
	if(wItemId[0] == 0)
	{
		g_pGVLogin->ChrSelectInfo[wIndex].hHandle = 
			CreateHandleObject(GetFile(RESTYPE_BASE_BODY, g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wClass)
			, GXPlayerPROC, g_pGVLogin->ChrSelectInfo[wIndex].pDesc, 0);
	}
	else
	{
		//갑옷이 있는 경우 //
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wMailID);

		if(lpItemResource)
		{
			ItemDataName(szInfo, wItemId[0], g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wClass-1);

			if(lpItemResource->wModCount>0)							
			{
				g_pGVLogin->ChrSelectInfo[wIndex].hHandle =
					CreateHandleObject(GetFile(szInfo, DATA_TYPE_CHARACTER), GXPlayerPROC, g_pGVLogin->ChrSelectInfo[wIndex].pDesc, 0);
			}
		}
	}		
	
	if(wItemId[1])
	{			
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId[1]);
		
		if(lpItemResource)
		{
			if(lpItemResource->wModCount>0)
			{	
				CBaseItem* pItemTable = g_pItemTableHash->GetData(wItemId[1]);
			
				if(pItemTable)
				{
					if(pItemTable->BaseItem_Weapon.bModCount==2)
					{
						ItemAttach(wItemId[1]
						, &g_pGVLogin->ChrSelectInfo[wIndex].hLeftHand
						, g_pGVLogin->ChrSelectInfo[wIndex].pLItemDesc
						, &g_pGVLogin->ChrSelectInfo[wIndex].hHandle, ITEM_ATTACH_TYPE_LHAND);
					}

					ItemAttach(wItemId[1]
						, &g_pGVLogin->ChrSelectInfo[wIndex].hRightHand
						, g_pGVLogin->ChrSelectInfo[wIndex].pLItemDesc
						, &g_pGVLogin->ChrSelectInfo[wIndex].hHandle, ITEM_ATTACH_TYPE_LHAND);
				}
			}						
		}
	}
	
	if(wItemId[2])
	{
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(wItemId[2]);
		
		if(lpItemResource)
		{			
			if(lpItemResource->wModCount > 0)
			{
				ItemAttach(wItemId[2], &g_pGVLogin->ChrSelectInfo[wIndex].hLeftHand
					, g_pGVLogin->ChrSelectInfo[wIndex].pLItemDesc, &g_pGVLogin->ChrSelectInfo[wIndex].hHandle, ITEM_ATTACH_TYPE_LHAND);
			}
		}
	}
		
	if(g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wClass < 4)
	{
		g_pGVLogin->ChrSelectInfo[wIndex].hHeadHandle
			= CreateHandleObject(GetFile(RESTYPE_HEAD_MALE, g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wHead)
			, NULL, g_pGVLogin->ChrSelectInfo[wIndex].pDesc, 0);
	}
	else
	{
		g_pGVLogin->ChrSelectInfo[wIndex].hHeadHandle 
			= CreateHandleObject(GetFile(RESTYPE_HEAD_FEMALE, g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wHead)
			, NULL, g_pGVLogin->ChrSelectInfo[wIndex].pDesc, 0);
	}

	g_pExecutive->SetData(g_pGVLogin->ChrSelectInfo[wIndex].hHeadHandle, g_pGVLogin->ChrSelectInfo[wIndex].pDesc);
	g_pExecutive->GXOAttach(g_pGVLogin->ChrSelectInfo[wIndex].hHeadHandle, g_pGVLogin->ChrSelectInfo[wIndex].hHandle, "Bip01 Head");
		
	// 헬멧 //
	if(g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wHelmetID != 0)
	{
		LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wHelmetID);

		if(lpItemResource)
		{
			if(lpItemResource->wModCount > 0)
			{
				ItemAttach(wItemId[3], &g_pGVLogin->ChrSelectInfo[wIndex].hHelmetHandle, NULL
				, &g_pGVLogin->ChrSelectInfo[wIndex].hHandle, ITEM_ATTACH_TYPE_HEAD);
			}
		}
	}		
	
	g_pGVLogin->ChrSelectInfo[wIndex].pDesc->dwTempBuf[0]	= wIndex;
	g_pGVLogin->ChrSelectInfo[wIndex].pDesc->dwTempBuf[1]	= wIndex;
	g_pGVLogin->ChrSelectInfo[wIndex].pDesc->pInfo			= &g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo;	//선택할 캐릭터 정보 
		
	// Action //
	if(g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wRHandID==0)
		::SetAction(g_pGVLogin->ChrSelectInfo[wIndex].hHandle, 1, 0, ACTION_LOOP);									
	else
	{
		int nIndex = g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo.wRHandID/ITEM_DISTRIBUTE;			
		::SetAction(g_pGVLogin->ChrSelectInfo[wIndex].hHandle, WORD(nIndex*50+MOTION_TYPE_VILLAGESTAND), 0, ACTION_LOOP);
	}		

	VECTOR3 v3;

	switch(wIndex)
	{
	case 0:		v3.x = 0;	v3.y = 0;	v3.z = 0;		break;
	case 1:		v3.x = 200;	v3.y = 0;	v3.z = 200;		break;
	case 2:		v3.x = 0;	v3.y = 0;	v3.z = 400;		break;
	case 3:		v3.x = -200;v3.y = 0;	v3.z = 200;		break;
	}

	GXSetPosition(g_pGVLogin->ChrSelectInfo[wIndex].hHandle, &v3, FALSE);
	_PlaySound(0, SOUND_TYPE_SYSTEM, SOUND_SYSTEM_CHARSELSPOT, g_v3InterfaceSoundPos, FALSE);
		
	if(wIndex==0)
	{
		DisplayCharacterInfo(&g_pGVLogin->ChrSelectInfo[wIndex].ReceivedChrInfo);
		g_pGVLogin->dwCurCharIndex = wIndex;

		ShowObject(g_pGVLogin->ChrSelectInfo[wIndex].hSpotLightHandle);

		::SetAction( g_pGVLogin->ChrSelectInfo[wIndex].hHandle, WORD((g_pGVLogin->ChrSelectInfo[wIndex].bByItemType) * 50 + MOTION_TYPE_ATTACK1_1), 0, ACTION_LOOP);

		g_pGVLogin->ChrSelectInfo[wIndex].pDesc->ObjectFunc = CharSelectHighlightMotion;
		g_pGVLogin->ChrSelectInfo[wIndex].pDesc->dwTempBuf[10] = 1;
	}

	g_byCharMoveSuccess++;	
	pCharMoveRtWnd->SetCharacter(pPacket->Character.szName, pPacket->byNum, 0);
	
	if(g_byCharMoveCnt==g_byCharMoveSuccess+g_byCharMoveFail)
	{
		pCharMoveRtWnd->m_byMoveFail	= g_byCharMoveFail;
		pCharMoveRtWnd->m_byMoveSuccess	= g_byCharMoveSuccess;
		pCharMoveRtWnd->m_byTotalCnt	= g_byCharMoveCnt;
		pCharMoveRtWnd->SetActive();
	}
}

void CmdMoveCharacterFail(char* pMsg, DWORD dwLen)
{
	ASTC_MOVE_CHARACTER_FAIL*	pPacket			= (ASTC_MOVE_CHARACTER_FAIL*)pMsg;
	CCharMoveRtWnd*				pCharMoveRtWnd	= CCharMoveRtWnd::GetInstance();

	g_byCharMoveFail++;
	pCharMoveRtWnd->SetCharacter(pPacket->szName, pPacket->byNum, pPacket->byType);

	if(g_byCharMoveCnt==g_byCharMoveSuccess+g_byCharMoveFail)
	{
		pCharMoveRtWnd->m_byMoveFail	= g_byCharMoveFail;
		pCharMoveRtWnd->m_byMoveSuccess	= g_byCharMoveSuccess;
		pCharMoveRtWnd->m_byTotalCnt	= g_byCharMoveCnt;
		pCharMoveRtWnd->SetActive();
	}
}

void AcmdConnectWorldServer( char* pMsg, DWORD dwLen )
{	
	ASTC_CONNECT_WORLD_SERVER*	pPacket		= (ASTC_CONNECT_WORLD_SERVER*)pMsg;
	CErrorWnd*					pErrorWnd	= CErrorWnd::GetInstance();
	
	char szIP[ MAX_IP_LENGTH ]	= {0,};
	char szStr[0xff]			= {0,};

	ChangeAddressType(pPacket->dwIp, szIP);
	
	if(!g_pNet->ConnectServer(szIP , pPacket->wPort, SERVER_INDEX_WORLD))
	{
		g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(FALSE);

		wsprintf(szStr,  "(ErrorCode:%d)", 22);
		
		//"월드서버 접속에 실패했습니다."
		// "운영팀에 문의하세요!"
		pErrorWnd->SetError(g_Message[ETC_MESSAGE485].szMessage, szStr, g_Message[ETC_MESSAGE483].szMessage, 2);
		pErrorWnd->m_bExit = TRUE;
		pErrorWnd->SetActive(TRUE);
	}

	// 일단 처음 월드로 들어갈때 이걸로 한다
	g_helpLoadingSprite.Load(10001);
	g_helpLoadingSprite.SetLoadingFlag(TRUE);

	//	deathrain - Add 2004. 7. 14
	Begin_NTO();
	//

	CTWS_WORLD_LOGIN packet;
	packet.dwPropID		= pPacket->dwPropID;
	packet.dwChar_Index = pPacket->dwChar_Index;
	packet.dwSerialCode = pPacket->dwSerialCode;
	packet.bEventFlag	= pPacket->bEventFlag;
	
	g_pNet->SendMsg((char*)&packet, packet.GetPacketSize(), SERVER_INDEX_WORLD );	
}

void CmdCreateCharacterSuccess( char* pMsg, DWORD dwLen )
{	
	WSTC_CREATE_CHARACTER_SUCCESS* pPacket = (WSTC_CREATE_CHARACTER_SUCCESS*)pMsg;

	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();			
	pGameDefaultWnd->SetActive(FALSE);	

	WORD wSex =		  HIWORD(g_pGVLogin->dwCurHeadIndexforCreate);
	WORD wHeadIndex = LOWORD(g_pGVLogin->dwCurHeadIndexforCreate);
	
	//캐릭터 생성할때 보여줄 모든 몸통 모델 Load
	LPObjectDesc pDesc = AllocObjDesc();
		
	//새로운 캐릭터의 몸생성 
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle = CreateHandleObject(GetFile(RESTYPE_BASE_BODY, g_pGVLogin->bCurClassIndexForCreate), GXPlayerPROC, pDesc, NULL);
	
	//머리생성 
	if(wSex)
		g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHeadHandle = 
			CreateHandleObject(GetFile(RESTYPE_HEAD_MALE, 1001 + wHeadIndex), NULL, NULL, NULL);		
	else
		g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHeadHandle = 
			CreateHandleObject(GetFile(RESTYPE_HEAD_FEMALE, 1001 + wHeadIndex), NULL, NULL, NULL);	
	
	//기본무기 장착 
	LP_ITEM_RESOURCE_EX lpItemResource = g_pItemResourceHash->GetData(1);
		
	if(lpItemResource)
	{
		if(lpItemResource->wModCount>0)
		{	
//			LP_ITEM_TABLE pItemTable = g_pItemTableHash->GetData(1);
			CBaseItem* pItemTable = g_pItemTableHash->GetData(1);
			
//			char szInfo[0xff] = {0,};

			if(pItemTable)
			{
				if(pItemTable->BaseItem_Weapon.bModCount==2)
				{
					ItemAttach(1
						, &g_pGVLogin->ChrSelectInfo[g_pGVLogin->dwCurCharIndex].hLeftHand
						, g_pGVLogin->ChrSelectInfo[g_pGVLogin->dwCurCharIndex].pLItemDesc
						, &g_pGVLogin->ChrSelectInfo[g_pGVLogin->dwCurCharIndex].hHandle
						, ITEM_ATTACH_TYPE_LHAND);
				}
					
				ItemAttach(1
					, &g_pGVLogin->ChrSelectInfo[g_pGVLogin->dwCurCharIndex].hRightHand
					, g_pGVLogin->ChrSelectInfo[g_pGVLogin->dwCurCharIndex].pRItemDesc
					, &g_pGVLogin->ChrSelectInfo[g_pGVLogin->dwCurCharIndex].hHandle
					, ITEM_ATTACH_TYPE_LHAND);
			}
		}	
	}	

	// 기본무기 1번
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].bByItemType = (1/ITEM_DISTRIBUTE) + 1;
	
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].pDesc = 
		(LPObjectDesc)g_pExecutive->GetData(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle);

	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].pDesc->pInfo = &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo;

	g_pExecutive->GXOAttach(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHeadHandle, 
								g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle, "Bip01 Head");

	g_pExecutive->GXODetach(g_pGVLogin->EnumCharForCreate[ g_pGVLogin->bCurClassIndexForCreate - 1 ], g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);
	HideObject(g_pGVLogin->EnumCharForCreate[ g_pGVLogin->bCurClassIndexForCreate - 1 ]);
	HideObject(g_pGVLogin->EnumCharHeadForCreate[wSex][wHeadIndex]);

	g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_NORMAL;

	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.dwLevel = 1;
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.wClass = g_pGVLogin->bCurClassIndexForCreate;
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.dwStr = pPacket->dwStr;
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.dwVit = pPacket->dwVit;
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.dwDex = pPacket->dwDex;
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.dwInt = pPacket->dwInt;
	g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.dwEgo = pPacket->dwEgo;

	__lstrcpyn(g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo.szName, g_pInputManager->GetInputBuffer(INPUT_BUFFER_CHARACTER_NAME1), MAX_CHARACTER_NAME_REAL_LENGTH); 

	DisplayCharacterInfo( &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo );
	pDesc->dwTempBuf[0] = g_pGVLogin->dwCurCharIndex;
	pDesc->dwTempBuf[1] = 0;	//현재 맨 앞에 나와있으니깐..  Roate위치는 0이겠지..

	SPR(BUTTON_DELETE)->ShowSprite(TRUE);
	SPR(BUTTON_CREATE)->ShowSprite(FALSE);
	HideCreateWindow();

	SPR(SPR_SELECTCHAR_VILL1_1)->ShowSprite(FALSE);
	SPR(SPR_SELECTCHAR_VILL1_2)->ShowSprite(FALSE);
	SPR(SPR_SELECTCHAR_VILL1_3)->ShowSprite(FALSE);
	SPR(SPR_SELECTCHAR_VILL2_1)->ShowSprite(FALSE);
	SPR(SPR_SELECTCHAR_VILL2_2)->ShowSprite(FALSE);
	SPR(SPR_SELECTCHAR_VILL2_3)->ShowSprite(FALSE);	

#ifdef _USE_IME	
	GET_IMEEDIT()->DeactivateIME(TRUE);
#endif
	
	g_byCharacterAllCount++;
}

void CmdCharacterSelectFail( char* pMsg, DWORD dwLen )
{
	ASTC_CHARACTER_SELECT_FAIL*	pPacket		= (ASTC_CHARACTER_SELECT_FAIL*)pMsg;
	CErrorWnd*					pErrorWnd	= CErrorWnd::GetInstance();
	char						szStr[0xff] = {0,};

	g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(FALSE);
	
	wsprintf(szStr,  "(ErrorCode:%d)",pPacket->bErrorCode);

	// "캐릭터 초기화에 실패했습니다.",
	// "운영팀에 문의하세요!"
	pErrorWnd->SetError(g_Message[ETC_MESSAGE482].szMessage, szStr, g_Message[ETC_MESSAGE483].szMessage, 2);	
	pErrorWnd->m_bExit = TRUE;
	pErrorWnd->SetActive(TRUE);
}

void AcmdItemmallInfo( char* pMsg, DWORD dwLen )
{
	ASTC_ITEMMALL_INFO* pPacket			= (ASTC_ITEMMALL_INFO*)pMsg;
	CWorldWnd*			pWnd			= CWorldWnd::GetInstance();
	CErrorWnd*			pErrorWnd		= CErrorWnd::GetInstance();
	char				szText[0xff]	= {0, };
	
	pWnd->m_bInfoType = UPDATE_GAME_CHAR_SELECT;	

	if(pPacket->bSuccess)
	{
		wsprintf(pWnd->szMessageLine[0], g_Message[ETC_MESSAGE835].szMessage, pPacket->bItemAllCount, pPacket->bItemCount); // "아이템몰에서 구입한 선물 %d개중 %d개의"
		lstrcpy(pWnd->szMessageLine[1], g_Message[ETC_MESSAGE836].szMessage); // "선물이 창고의 빈공간에 저장되었습니다."
		lstrcpy(pWnd->szMessageLine[2], g_Message[ETC_MESSAGE837].szMessage); // "아이템에관한  자세한 내역은 홈페이지의"
		lstrcpy(pWnd->szMessageLine[3], g_Message[ETC_MESSAGE838].szMessage); // "아이템몰을 참고하세요."
	}
	else
	{
		if(pPacket->byItemMallType==0xff)
		{
			wsprintf(szText, g_Message[ETC_MESSAGE840].szMessage, pPacket->bItemCount);

			// "캐릭터가 없어서 저장을 할수 없습니다."
			// "캐릭터를 생성해 주세요."
			pErrorWnd->SetError(szText, g_Message[ETC_MESSAGE1086].szMessage, g_Message[ETC_MESSAGE1087].szMessage, 3); 
			pErrorWnd->SetActive(TRUE);
			return;
		}
		else if(pPacket->bItemCount == 0xff)
		{			
			// "아이템몰 내역 저장에 실패했습니다."
			// "운영팀에 문의하세요!"
			pErrorWnd->SetError(g_Message[ETC_MESSAGE839].szMessage, g_Message[ETC_MESSAGE483].szMessage, "", 2);
			pErrorWnd->SetActive(TRUE);
			return;
		}
		else
		{
			wsprintf(pWnd->szMessageLine[0], g_Message[ETC_MESSAGE840].szMessage, pPacket->bItemCount); // "아이템몰에서 %d개의 선물이 도착했으나"
			lstrcpy(pWnd->szMessageLine[1], g_Message[ETC_MESSAGE841].szMessage); // "유저님의 창고가 가득차서 아이템을"
			lstrcpy(pWnd->szMessageLine[2], g_Message[ETC_MESSAGE842].szMessage); // "저장 할 수 없습니다."
			lstrcpy(pWnd->szMessageLine[3], g_Message[ETC_MESSAGE843].szMessage); // "마을에서 창고를 비워주세요!"
		}
	}
	pWnd->m_fPosX		= 300;
	pWnd->m_fPrevPosX	= pWnd->m_fPosX;
	pWnd->m_fPosZ		= 200;
	pWnd->m_fPrevPosZ	= pWnd->m_fPosZ;
	pWnd->SetActive();
}

void CmdDeleteCharacterResult( char* pMsg, DWORD dwLen )
{
	WSTC_DELETE_CHARACTER_RESULT*	pPacket		= (WSTC_DELETE_CHARACTER_RESULT*)pMsg;
	CErrorWnd*						pErrorWnd	= CErrorWnd::GetInstance();
	
	if(pPacket->bSuccess)
	{
		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hLeftHand, &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].pLItemDesc);

		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hRightHand, &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].pRItemDesc);

		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHeadHandle, &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle);
		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHelmetHandle, &g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle);

		ItemDeleteObject(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].hHandle, NULL);
		ObjDeleteDesc(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].pDesc);		
				
		memset(&g_pGVLogin->ChrSelectInfo[ g_pGVLogin->dwCurCharIndex ].ReceivedChrInfo, 0, sizeof(RS_CharacterLogInfo));

		g_pInputManager->ClearInput(INPUT_BUFFER_CHARACTER_NAME1);		

		DisplayCharacterInfo(NULL);
		SPR(BUTTON_CREATE)->ShowSprite(TRUE);
		SPR(BUTTON_DELETE)->ShowSprite(FALSE);
		SPR(BUTTON_DELETE_OVER)->ShowSprite(FALSE);
		SPR(BUTTON_DELETE_DOWN)->ShowSprite(FALSE);

		HideObject(g_pGVLogin->ChrSelectInfo[ 0 ].hSpotLightHandle);	//Spot Light Off

		g_byCharacterAllCount--;
	}
	else
	{
		switch(pPacket->nResultValue)
		{
		case -101:
		case -100:	//쿼리에러 
			{
				pErrorWnd->SetError(g_Message[ETC_MESSAGE793].szMessage, // "캐릭터 삭제에 실패했습니다."
				g_Message[ETC_MESSAGE794].szMessage,	// "데이터베이스 통신오류"
				g_Message[ETC_MESSAGE483].szMessage, 3); // "운영팀에 문의하세요!"
				pErrorWnd->m_bExit = FALSE;
				pErrorWnd->SetActive(TRUE);
			}
			break;
		case 4:
			{
				pErrorWnd->SetError(g_Message[ETC_MESSAGE793].szMessage, // "캐릭터 삭제에 실패했습니다."
				g_Message[ETC_MESSAGE795].szMessage, // "비정상적인 시도입니다."
				g_Message[ETC_MESSAGE483].szMessage, 3); // "운영팀에 문의하세요!"
				pErrorWnd->m_bExit = TRUE;
				pErrorWnd->SetActive(TRUE);
			}
			break;
		case 5:
			{
				pErrorWnd->SetError(g_Message[ETC_MESSAGE793].szMessage, // "캐릭터 삭제에 실패했습니다."
				g_Message[ETC_MESSAGE849].szMessage, //"주민등록번호가 틀립니다."				
				g_Message[ETC_MESSAGE483].szMessage, 3); // "운영팀에 문의하세요!"
				pErrorWnd->SetActive(TRUE);
			}
			break;
		case 10:
			{
				pErrorWnd->SetError(g_Message[ETC_MESSAGE1088].szMessage, // "캐릭터를 삭제 후 24시간 이내에는"
					g_Message[ETC_MESSAGE1089].szMessage, NULL, 2); // "재생성을 하실수가 없습니다."
				pErrorWnd->SetActive(TRUE);
			}
			break;
		}
	}
	g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_NORMAL;	
}

void CmdWorldLoginFail( char* pMsg, DWORD dwLen )
{
	/* [Error Code]
	1. 등록된 시리얼번호 아님
	2. AcceptList에 없는 유저가 로그인한 경우 
	3. Recent Dungeon 정보가 존재하지 않음
	4. 월드맵상에서 시작위치를 찾을 수 없음
	*/

	WSTC_WORLD_LOGIN_FAIL *pPacket = (WSTC_WORLD_LOGIN_FAIL*)pMsg;

	g_pSprManager->GetSprite(SPR_LOADING_BACK)->ShowSprite(FALSE);

	CErrorWnd*	pErrorWnd		= CErrorWnd::GetInstance();
	char		szStr[0xff]		= {0,};
	char		szStr2[0xff]	= {0,};
	
	switch(pPacket->bErrorCode)
	{
	case 1:	lstrcpy(szStr, g_Message[ETC_MESSAGE689].szMessage);	// "유저 인증에 실패했습니다."
			lstrcpy(szStr2, g_Message[ETC_MESSAGE752].szMessage);	// "시리얼번호가 틀립니다."
			break;
	case 2: lstrcpy(szStr, g_Message[ETC_MESSAGE689].szMessage);	// "유저 인증에 실패했습니다."
			lstrcpy(szStr2, g_Message[ETC_MESSAGE753].szMessage);	// "승인목록에 없는 유저입니다."
			break;
	case 3: lstrcpy(szStr, g_Message[ETC_MESSAGE747].szMessage);	// "최근 로그아웃한 던전정보를"
			lstrcpy(szStr2, g_Message[ETC_MESSAGE165].szMessage);	// " 찾을 수 없습니다."
			break;
	case 4: lstrcpy(szStr, g_Message[ETC_MESSAGE754].szMessage);	// "월드맵의 시작위치를 찾을 수 "
			lstrcpy(szStr2, g_Message[ETC_MESSAGE4].szMessage);		// "없습니다."
			break;
	default:wsprintf(szStr, "(ErrorCode:%d)",pPacket->bErrorCode);	memset(szStr2, 0, 0xff);	break;
	}
	pErrorWnd->SetError(szStr, szStr2, g_Message[ETC_MESSAGE483].szMessage,	3);		
	pErrorWnd->m_bExit = TRUE;
	pErrorWnd->SetActive();
}

void CmdCreateCharacterFail( char* pMsg, DWORD dwLen )
{	
	WSTC_CREATE_CHARACTER_FAIL *pPacket = (WSTC_CREATE_CHARACTER_FAIL*)pMsg;

	CGameDefaultWnd* pGameDefaultWnd = CGameDefaultWnd::GetInstance();			
	pGameDefaultWnd->SetActive(FALSE);	
	
	switch(pPacket->nResultValue)
	{
	case -1:
		{			
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE13].szMessage, "", "", 384, 320, 1);	// MSG_ID : 13 ; 이미 사용중인 이름입니다.
			pGameDefaultWnd->SetActive(TRUE);
		}
		break;
	case -2:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE14].szMessage, g_Message[ETC_MESSAGE15].szMessage, "", 384, 320, 2);	// MSG_ID : 14 ; 캐릭터의 자리가 중복됩니다., 15 ; 고객감동팀에게 문의하세요! 
			pGameDefaultWnd->SetActive(TRUE);			
		}
		break;
		
	case -3: // 중국의 경우, 로그인 서버에서도 금지단어 체크를 한다...
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE59].szMessage, "", "", 384, 320, 1);	// MSG_ID : 14 ; 캐릭터의 자리가 중복됩니다., 15 ; 고객감동팀에게 문의하세요! 
			pGameDefaultWnd->SetActive(TRUE);
		}
		break;

	default:
		{
			pGameDefaultWnd->OpenWnd(g_Message[ETC_MESSAGE248].szMessage, g_Message[ETC_MESSAGE18].szMessage, "", "", 384, 320, 1); // MSG_ID : 18 ; 캐릭터 생성에 실패했습니다.
			pGameDefaultWnd->SetActive(TRUE);			
		}		
		break;
	}	
	// 다시 캐릭터 생성 시 시작마을 고를 수 있게!!
	g_bCharacterChk = TRUE;
	g_pGVLogin->dwFlag = CHAR_SELECT_STATUS_CHAR_MAKING;
	g_pInputManager->SetFocusInput(INPUT_BUFFER_CHARACTER_NAME1);
} 
