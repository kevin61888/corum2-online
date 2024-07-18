#if !defined(AFX_DUNGEON_DATA_H__184463A9_2436_4FEC_849B_A2604FF03BA6__INCLUDED_)
#define AFX_DUNGEON_DATA_H__184463A9_2436_4FEC_849B_A2604FF03BA6__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "../CommonServer/CommonHeader.h"
#include "GlobalDefine.h"


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
	WORD	m_wAreaLevel_h;								// 0 = 왕초보존, 1 = 초보존, 2 = 중수존, 3 = 고수존, 4 = 초절정고수존
	WORD	m_wAreaLevel_l;
	WORD	m_wInitLevel;								// 던전 기본 레벨
	WORD	m_wUpgradeLevel;							// 던전 업그레이드된 레빌
	DWORD	m_wBattleTimeLimit;							// 공성전 제한 시간.
	WORD	m_wProduction;								// 생산품 아이디.
	BYTE	m_byProductionCount;						// 생산품 갯수.		
	BYTE	m_bSiege;									// 공성중 : 1
	BYTE	m_bOperationType;							// 던전 운영 상태..0 = 노멀모드, 1 = 방어모드, 2 = 산출모드, 3 = 레벨업모드
	WORD	m_wLayerCount;								// 구성된 레이어  층수.
	DWORD	m_dwOwnerIndex;								// 던전 주인 인덱스
	char	m_szOwner[ MAX_CHARACTER_NAME_LENGTH ];		// 소유주 또는 소유길드명 	
	char    m_szManager[ MAX_CHARACTER_NAME_LENGTH ];	// 관리자 캐릭터명..  길드소유인경우 디폴드는 길마
	DWORD	m_dwOwnerGuildNum;
	DWORD	m_dwEntrance;								// 입장료
	DWORD	m_dwAccEntrance;							// 누적된 입장료
	DWORD	m_dwAccExp;									// 누적 경험치
	CItem	m_cMagicFieldArrayItem;						// 마법진 아이템
	DWORD	m_dwLayerFormation[MAX_LAYER_PER_DUNGEON];
	CItem	m_cGuardianItem;							// 가디언 아이템	
	BYTE	m_byDefenseCount;							// 방어 성공 수
	BYTE	m_byProperty;								// 속성
	BYTE	m_byEntererMinLevel;						// 들어올놈의 최소 레벨
	BYTE	m_byEntererMaxLevel;						// 들어올놈의 최대 레벨
	BYTE	m_byCallGuardian;							// 수호 가디언 부를수 있냐?
};
#pragma pack(pop)


#endif 
