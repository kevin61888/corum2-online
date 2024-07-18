/*#####################################################################//
//##################### 2004. 11. 클래스화 예정.. #####################
//#####################################################################
//
//	Version		: Dev/Develop
//	ProjectName	: CorumOnlineProject
//	FileName	: RivalGuildWarLib.h
//	Date		: 2004. 10. 18. ( minjin )
//	Description	: Declare Rival Guild War Global Function
//				  For Assistant to Set or Get Information About Guild
//
//#####################################################################*/

#ifndef __RIVAL_GUILD_WAR_LIB__
#define __RIVAL_GUILD_WAR_LIB__

#include "WorldUser.h"
#include "../CommonServer/AbyssHash.h"

//#define MIN_RGW_GUILD_LEVEL		5
//#define MIN_RGW_GUILD_MEMBER	5

enum EN_RivalGuildWarErrorCode
{
	RIVAL_GUILD_WAR_ERRCODE_DUMMY	= 0,	// 라이벌 길드전 에러코드 더미.. -_-;;;

	RGWE_VS_NOT_GUILD_MASTER		= 1,	// "상대방이 길드마스터가 아닙니다."
	RGWE_VS_MUST_LEVEL5				= 2,	// "상대 길드 레벨 5이상이어야 라이벌 길드전이 가능합니다."
	RGWE_VS_MORE_GUILD_MEMBER		= 3,	// "상대 길드마스터를 제외한 길드원이 최소 5명 이상 존재해야 합니다."
	RGWE_VS_NOT_ACCEPT_WAR			= 4,	// "%s 길드 마스터가 라이벌 길드전 선포 신청을 거절하였습니다."

	RGWE_NOT_GUILD_MASTER			= 11,	// "길드 마스터만 라이벌 길드전을 신청할 수 있습니다."
	RGWE_MUST_LEVEL5				= 12,	// "길드 레벨 5이상이어야 라이벌 길드전이 가능합니다."
	RGWE_MORE_GUILD_MEMBER			= 13,	// "길드마스터를 제외한 길드원이 최소 5명 이상 존재해야 합니다."

	RGWE_NOW_ON_GUILD_WAR			= 21,	// "현재 라이벌 길드전이 진행되고 있습니다."
	RGWE_SAME_GUILD					= 22,	// "같은 길드원끼리 라이벌 길드전을 신청할 수 없습니다."
	RGWE_CANNOT_REMOVE_GUILD		= 23,	// "라이벌 길드전 중 길드삭제가 불가능 합니다."
	RGWE_NOW_WAIT_ACCEPT			= 24,	// "현재 라이벌 길드전 협상중입니다. 잠시 후에 다시 시도해주세요."

	RGWE_GUILD_DATA_ERROR			= 100,	// 라이벌 길드전 데이터 없을때.. -_-;;;

	RGWE_GUILD_WAR_SUCCESS			= 255,
};

//BYTE	RGWLibCheckCondition( DWORD dwMyGuildIdx, DWORD dwOpGuildIdx );

VOID	ConfirmDestRGWDungeon( DWORD* dwDungeonID, DWORD* dwDestLayer );

/*
CWorldUser* IsThereGuildMaster(ListNode<CWorldUser>* pGuildUserPos);
//*/


#endif
