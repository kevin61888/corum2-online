/*#####################################################################//
//##################### 2004. 11. 클래스화 예정.. #####################
//#####################################################################
//
//	Version		: Dev/Develop
//	ProjectName	: CorumOnlineProject
//	FileName	: RivalGuildWarLib.cpp
//	Date		: 2004. 10. 18. ( minjin )
//	Description	: Define Rival Guild War Global Function
//				  For Assistant to Set or Get Information About Guild
//
//#####################################################################*/

#include "StdAfx.h"
#include "Guild.h"
#include "GuildHash.h"
#include "RivalGuildWarLib.h"

/*	minjin.
 *	우선은, 라이벌 길드전 관련 함수들 여기다가 모아놓는다.
 *	길드거를 그냥 가져다 쓰던, 암튼 여기다 모아다 놓고,
 *	나중에 클래스로 바꿀때.. 싹 갈아엎어버린다.. -_-;;;
 */

//------------------------------------------------------------------------
// Date			: 2004. 10. 19. ( Written by minjin )
// Description	: 라이벌 길드전 할수 있는지 없는지 검사
//				  할 수 있으면 RGWE_GUILD_WAR_SUCCESS
//				  할수 없으면, EN_RivalGuildWarErrorCode 값 리턴
//------------------------------------------------------------------------
// 길드전 조건
// 1. 길드 레벨이 5 이상이어야 함
// 2. 길드 마스터가 접속되어 있어야 함
// 3. 길드마스터를 제외한 나머지 길드원이 최소 5명 이상 있어야 함
// 4. 패배한 후 24 시간이 지나야 게임에 참여 가능
//------------------------------------------------------------------------
/* 
BYTE RGWLibCheckCondition( DWORD dwMyGuildIdx, DWORD dwOpGuildIdx )
{
//	LP_GUILD_LIST	lpGuildList		= g_pGuildWarHash->GetData(dwMyGuildIdx);	// 신청한 길드
//	LP_GUILD_LIST	lpGuildListDst	= g_pGuildWarHash->GetData(dwOpGuildIdx);	// 상대 길드

	LP_GUILD_TABLE	lpGuildTable	= g_pGuildTableHash->GetData(dwMyGuildIdx);	// 신청한 길드
	LP_GUILD_TABLE	lpGuildTableDst	= g_pGuildTableHash->GetData(dwOpGuildIdx);	// 상대 길드


/ *	if( !lpGuildList )			// 신청한 길드의 길드리스트.. 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildList is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildListDst )		// 상대 길드의 길드리스트.. 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildListDst is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}* /


	if( !lpGuildTable )			// 신청한 길드의 길드테이블이 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildTable is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}

	if( !lpGuildTableDst )		// 상대 길드의 길드테이블이 이상..?
	{
		Log( LOG_NORMAL, "[minjin] lpGuildTableDst is Invalidate. Check the GuildList TABLE" );
		return RGWE_GUILD_DATA_ERROR;
	}

	CWorldUser* pMyMasterUser = IsThereGuildMaster( lpGuildTable->pGuildUserHash->GetHead() );

	if( !pMyMasterUser )														return RGWE_NOT_GUILD_MASTER;
//	if( lpGuildTable->sGuild.wLevel < MIN_RGW_GUILD_LEVEL )						return RGWE_MUST_LEVEL5;

	
#if defined(JAPAN_LOCALIZING)
	if( lpGuildTable->pGuildUserHash->GetCount() <= MIN_RGW_GUILD_MEMBER )		return RGWE_MORE_GUILD_MEMBER;
#endif


	CWorldUser* pOpMasterUser = IsThereGuildMaster( lpGuildTableDst->pGuildUserHash->GetHead() );

	if( !pOpMasterUser )														return RGWE_VS_NOT_GUILD_MASTER;
//	if( lpGuildTableDst->sGuild.wLevel < MIN_RGW_GUILD_LEVEL )					return RGWE_VS_MUST_LEVEL5;

	
#if defined(JAPAN_LOCALIZING)
	if( lpGuildTableDst->pGuildUserHash->GetCount() <= MIN_RGW_GUILD_MEMBER )	return RGWE_VS_MORE_GUILD_MEMBER;
#endif


	// 모두 무사통과.
	// 라이벌 길드전 해라..
	return RGWE_GUILD_WAR_SUCCESS;
}*/


VOID	ConfirmDestRGWDungeon( DWORD* dwDungeonID, DWORD* dwDestLayer )
{
	// 우선 5500 번으로 한다!
	*dwDungeonID = 5500;
	*dwDestLayer = 0;
}
/*
//------------------------------------------------------------------------
// Date			: 2004. 09. 24. ( Written by minjin )
// Description	: 길드 마스터가 있으면 길마 Pointer / 없으면 FALSE
//------------------------------------------------------------------------
CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos)
{
	CWorldUser*	pMasterUser	= NULL;

	while ( pGuildUserPos ) 
	{
		pMasterUser = pGuildUserPos->pData;

		if ( pMasterUser ) 
		{
			if ( pMasterUser->m_byRank == 1 ) 
			{
				return pMasterUser;		// 길마 Pointer 리턴!
			}
		}
		else
		{
			return FALSE;
		}
		pGuildUserPos = pGuildUserPos->pNext;
	}

	// 리스트 끝까지 검색한거같은데.. 그렇다면..
	// 허거!! 길마가 엄따..!!! -_-;;
	return FALSE;
}//*/
