#pragma once

#include "stdafx.h"
#include "Matcher.h"
#include "WorldUserTable.h"

#include "DungeonTable.h"

struct MATCH_CONDITION_PARTY
{
	BYTE bLevel;	// 레벨
	BYTE bExp;		// 경험치 분배
	BYTE bItem;		// 아이템 분배
};

bool operator<(const MATCH_CONDITION_PARTY& f, const MATCH_CONDITION_PARTY& b);

class CMatcherActionParty
{

// 제네릭 매쳐의 조건 검사자 및 매칭자 : 최덕석 2005.1.29
public:
	// 조건 검사 및 매칭자
	void DoMatch(DWORD dwOwnUserIndex, DWORD dwGuestUserIndex, DWORD dwMatchIndex)
	{
		CWorldUser* pOwnUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwOwnUserIndex);
		CWorldUser* pGuestUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwGuestUserIndex);

		if(pOwnUser == NULL || pGuestUser == NULL)
			return;

		// 두 대상에게 대기실 번호를 알려줌
		pOwnUser->m_dwPMatchIndex = dwMatchIndex;
		pGuestUser->m_dwPMatchIndex = dwMatchIndex;

		// 파장에게 매칭을 승낙을 물어봄
		WSTC_PMATCH_REQUEST PMatchRequest;
		PMatchRequest.bLevel = (BYTE)pGuestUser->m_dwLevel;
		PMatchRequest.bClass = (BYTE)pGuestUser->m_wClass;
		strcpy(PMatchRequest.szName, pGuestUser->m_szCharacterName);
		PMatchRequest.bMaster = 1;

		g_pNet->SendToUser(pOwnUser->m_dwConnectionIndex, (char*)&PMatchRequest, PMatchRequest.GetPacketSize() , FLAG_SEND_NOT_ENCRYPTION);
	}

	// (비슷한) 조건 나열자
	bool MakeCondition(DWORD dwUserIndex, MATCH_CONDITION_PARTY& cnd, MATCH_CONDITION_PARTY& out)
	{
		CWorldUser* pUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwUserIndex);

		if(pUser == NULL)
			return false;

		// 대상의 의뢰 조건(레벨)의 범위가 넘었다면
		if(pUser->m_PMatchCondition.bLevel < pUser->m_bLevelDist)
		{
			// 조건 변수들 초기화
			pUser->m_bLevelDist = 0;
			pUser->m_iSign = 1;

			// 더이상 (비슷한) 조건이 없음을 알려줌
			return false;
		}

		// 새로 만든 조건을 알려줌
		out = cnd;
		
		out.bLevel = (BYTE)(pUser->m_dwLevel + pUser->m_bLevelDist * pUser->m_iSign);
		
		pUser->m_iSign *= -1;
		if(pUser->m_iSign == -1)
			pUser->m_bLevelDist++;

		return true;
	}

	// 매칭 취소 핸들러
	void CancelMatch(DWORD dwOwnUserIndex, DWORD dwGuestUserIndex)
	{
		CWorldUser* pOwnUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwOwnUserIndex);
		CWorldUser* pGuestUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwGuestUserIndex);

		if(pOwnUser == NULL)
		{
			pOwnUser->m_dwPMatchIndex = 0;
		}

		if(pGuestUser == NULL)
		{
			pGuestUser->m_dwPMatchIndex = 0;
		}
	}

	// 매칭 가능한지 알아냄
	bool CanMatch(DWORD dwOwnUserIndex, DWORD dwGuestUserIndex)
	{
		
		CWorldUser* pOwnUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwOwnUserIndex);
		CWorldUser* pGuestUser = (CWorldUser*)g_pUserTable->GetUserInfo(dwGuestUserIndex);
		DUNGEON_DATA_EX *pDungeonForOwn, *pDungeonForGuest;

		// 패킷 손실로, 같은 대상을 매칭 하려면
		if(dwOwnUserIndex == dwGuestUserIndex)
			return false;

		// 둘중 어느 누구라도 나갔다면
		if(pOwnUser == NULL || pGuestUser == NULL)
			return false;

		// 게스트가 파장을 원한다면
		if(pGuestUser->m_PMatchCondition.bMaster == 1)
			return false;

		// 게스트가 파티에 참여 중이면
		if(pGuestUser->m_dwPartyId != 0)
			return false;

		// 레벨차를 계산
		int iLevelDist = (int)abs((int)pOwnUser->m_dwLevel - (int)pGuestUser->m_dwLevel);
		
		// 원하는 레벨차가 벗어났으면
		if(iLevelDist > pOwnUser->m_PMatchCondition.bLevel
		|| iLevelDist > pGuestUser->m_PMatchCondition.bLevel)
			return false;
	
		// 속한 던전을 알아냄
		pDungeonForOwn = g_pDungeonTable->GetDungeonInfo((WORD)pOwnUser->m_dwDungeonID);
		pDungeonForGuest = g_pDungeonTable->GetDungeonInfo((WORD)pGuestUser->m_dwDungeonID);

		// 유저가 월드에 있다면
		if( pDungeonForOwn == NULL ||  pDungeonForGuest == NULL)
			return false;

		// 길드전 중이면
		if(pDungeonForOwn->IsStadium() || pDungeonForGuest->IsStadium())
			return false;

		// 이벤트 던전이면
		if(pDungeonForOwn->GetDungeonType() == DUNGEON_TYPE_EVENT || pDungeonForGuest->GetDungeonType() == DUNGEON_TYPE_EVENT)
			return false;

		return true;
	}
};

extern CMatcher<MATCH_CONDITION_PARTY, DWORD, CMatcherActionParty> g_PartyMatcher;