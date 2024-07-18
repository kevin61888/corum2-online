// DUNGEON_DATA.h: interface for the DUNGEON_DATA class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DUNGEON_DATA_H__937E4CB7_8A67_4DA5_BB5C_E12286084706__INCLUDED_)
#define AFX_DUNGEON_DATA_H__937E4CB7_8A67_4DA5_BB5C_E12286084706__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Define.h"
#include "../CommonServer/CommonHeader.h"


#define TIME_SEC		1
#define	TIME_MIN		2
#define	TIME_HOUR		3
#pragma pack(push, 1)
struct DUNGEON_DATA
{
	DWORD	m_dwID;
	
	WORD	m_wGroupID;
	char	m_szDungeonName[ MAX_DUNGEON_NAME_LENGTH ];
	WORD	m_wAreaNum;
	WORD	m_wAreaLevel_h;	//0 = 왕초보존, 1 = 초보존, 2 = 중수존, 3 = 고수존, 4 = 초절정고수존
	WORD	m_wAreaLevel_l;
	WORD	m_wInitLevel;		// 던전 기본 레벨
	WORD	m_wUpgradeLevel;	// 던전 업그레이드된 레빌
	DWORD	m_wBattleTimeLimit;	// 공성전 제한 시간.
	WORD	m_wProduction;	// 생산품 아이디.
	BYTE	m_byProductionCount;	// 생산품 갯수.	
	BYTE	m_bSiege;		//공성중 : 1
	BYTE	m_bOperationType;	// 던전 운영 상태..0 = 노멀모드, 1 = 방어모드, 2 = 산출모드, 3 = 레벨업모드
	WORD	m_wLayerCount;		// 구성된 레이어  층수.
	DWORD	m_dwOwnerIndex;		// 주인 인덱스
	

	char	m_szOwner[ MAX_CHARACTER_NAME_LENGTH ];		//소유주 또는 소유길드명 	
	char    m_szManager[ MAX_CHARACTER_NAME_LENGTH];	//관리자 캐릭터명..  길드소유인경우 디폴드는 길마
	
	char	m_szMemo[ MAX_DUNGEON_MEMO_LENGTH ];		//던전 관리자의 메모 
	DWORD	m_dwOwnerGuildNum;
	CItem	m_cMagicFieldArrayItem;		// 마법진 아이템
	DWORD	m_dwEntrance;				// 입장료
	DWORD	m_dwAccEntrance;			// 누적된 입장료
	DWORD	m_dwAccExp;			// 누적된 경험치
	CItem	m_cGuardianItem;			// 가디언 아이템	
	BYTE	m_byDefenseCount;			// 디펜스 횟수
	BYTE	m_byProperty;					// 속성
	BYTE	m_byEntererMinLevel;			// 들어올놈의 최소 레벨
	BYTE	m_byEntererMaxLevel;			// 들어올놈의 최대 레벨

	
};
#pragma pack(pop)
#endif // !defined(AFX_DUNGEON_DATA_H__937E4CB7_8A67_4DA5_BB5C_E12286084706__INCLUDED_)
