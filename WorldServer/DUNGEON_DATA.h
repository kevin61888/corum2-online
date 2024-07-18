#if !defined(AFX_DUNGEON_DATA_H__4320E585_0233_49AB_A72D_5FE1E50B5D53__INCLUDED_)
#define AFX_DUNGEON_DATA_H__4320E585_0233_49AB_A72D_5FE1E50B5D53__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "../CommonServer/CommonHeader.h"


#pragma pack(push,1)
struct DUNGEON_DATA
{
	DWORD	m_dwID;										// 던전 아이디.
	WORD	m_wWorldServerNo;							// 이 던전이 어떤 월드서버에 속해있는 던전인지 구분해 주는 월드서버 번호 
	WORD	m_wWorldMapID;								// 월드맵 대륙번호, (주의)대륙번호가 다르다고 해서 월드서버가 다른것은 아니다. 
	WORD	m_wGroupID;									// 그룹 아이디.
	char	m_szDungeonName[MAX_DUNGEON_NAME_LENGTH];
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
	WORD	m_wLayerCount;								// 층갯수.
	DWORD	m_dwOwnerIndex;
	char	m_szOwnerID[MAX_CHARACTER_NAME_LENGTH ];	// 소유주의 로긴 아이디
	char	m_szOwner[MAX_CHARACTER_NAME_LENGTH];		// 소유주 또는 소유길드명 	
	char    m_szManager[MAX_CHARACTER_NAME_LENGTH];		// 관리자 캐릭터명..  길드소유인경우 디폴드는 길마
	char	m_szMemo[MAX_DUNGEON_MEMO_LENGTH];			// 던전 관리자의 메모 
	DWORD	m_dwOwnerGuildNum;
	CItem	m_cMagicFieldArrayItem;						// 마법진 아이템
	DWORD	m_dwPort;
	DWORD	m_dwEntrance;								// 입장료
	DWORD	m_dwAccEntrance;							// 누적된 입장료
	DWORD	m_dwAccExp;									// 누적된 경험치
	CItem	m_cGuardianItem;							// 가디언 아이템
	DWORD	m_dwPieceStartTime;							// 평화롭게 상태가 바뀌기 시작한 시간.
	DWORD	m_dwPieceCoolTime;							// 공성 주기
	DWORD	m_wBattleStartTime;							// 공성전 시작한 시간
	BYTE	m_byDefenseCount;							// 방어 성공횟수
	BYTE	m_byProperty;								// 속성
	BYTE	m_byEntererMinLevel;						// 들어올놈의 최소 레벨
	BYTE	m_byEntererMaxLevel;						// 들어올놈의 최대 레벨
	double	m_dWorldPos_X;		
	double	m_dWorldPos_Z;
};
#pragma pack(pop)


#endif
