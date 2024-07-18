#if !defined(AFX_DUNGEON_DATA_EX_H__71BCF2A8_A1AD_4074_A8D1_A703ECB4C452__INCLUDED_)
#define AFX_DUNGEON_DATA_EX_H__71BCF2A8_A1AD_4074_A8D1_A703ECB4C452__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif 


#include "ServerTable.h"
#include "OnlyList.h"
#include "dungeon_data.h"
#include "EventDungeonManager.h"
#include "../CommonServer/CommonDungeon.h"


#define SERVER_MESSAGE_SIEGE_CONQUER		1	// "%s님이 %s던전을 점령했습니다." 

struct EVENT_DUNGEON_PROPERTY;


class DUNGEON_DATA_EX : public DUNGEON_DATA
{
	
public:

	LONG						m_lDungeonType;				// 던젼 타입.
	DWORD						m_dwRemainSiegeStartTick;	// 공성전 할려면 몇시간 남았는가?
	DWORD						m_dwRemainSiegeEndTick;		// 공성전 방어하기 몇시간 남았는가?
	DWORD						m_dwRemainLevelStartTick;	// 던전 레벨 (시간이 지남에 따라 레벨이 증가한다.)
	DWORD						m_dwStopLevelTick;			// 멈추었을때 레벨업글되기까지의 남은시간.
	DWORD						m_dwStopStartTick;
	DWORD						m_dwStartLevelStartTick;	// 운영모드 바뀔때 지난시간을 알아야 하기에.
	DWORD						m_dwEventLastActionTick;	// 이벤트 던젼 마지막으로 액션 벌어진 시각.
	LONG						m_iEventEntered;			// 이벤트 던젼에 들어간 유저 수.
	BYTE						m_bEventState;				// 이벤트 던전 상태.

	char						m_szSchoolName[MAX_CHARACTER_NAME_LENGTH]; // 일본판의 경우, 소유 학교 길드 이름
	
	POSITION_					m_pListPos;
	SERVER_DATA*				m_pServer;					// 던전에 있는 서버 정보 		
	DUNGEON_DATA_EX*			m_pNextData;
	DUNGEON_DATA_EX*			m_pPrevData;
	CDugneonRefreshUserList*	m_pListSendToUser;			// 던전 정보가 갱신되었을때 유저들에게 보내야 한다.
	
	void						SetDungeonType(WORD wID);
	INT							GetDungeonType();
	WORD						GetLevel();
	WORD						GetUpgradeTime();
	int							GetIdleTime();
	void						SetSiegeStartDestTime(DWORD dwTick);
	void						SetSiegeEndDestTime(DWORD dwTick);
	BOOL						StartSiegeWarTime();
	BOOL						LevelUpgrade();
	void						SendRefreshProductionInfo(WORD wItemID, BYTE byProductionCount);
	BOOL						IsConquer();		
	BOOL						IsStartWarTime();
	BOOL						IsEndWarTime();
	BOOL						IsLevelUpgradeTime();
	DWORD						GetEntrancePay();	
	void						SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerID, char* pOwnerName);
	void						SendStartSiege();
	void						SendEndSiege(BYTE bDefenseSuccess);
	void						SendToUsers(char* pPacket, DWORD dwSize);
	void						SetOperationMode(BYTE bOperationMode);
	void						StopOperationTime();		
	void						ResumeOperationTime();		
	void						SetAccEntrance(int nPlusMoney, BOOL bSend = FALSE);
	DWORD						GetLevelUpTickCount();
	void						SendRefreshLevelUp();
	void						SendRefreshOwner();
	void						SetAccExp(DWORD dwExp);
	void						SendRefreshAccExp();
	void						SendRefreshEntrance();
	void						SetEntrance(DWORD dwEntrance);
	void						SendRefreshOwnerGuild();
	BOOL						EventChangeState(BYTE byState, DWORD dwCurrentTick);	
	BOOL						SendEventInfo(DWORD dwEventEndTick = 0);
	void						SendEventInfoClose();
	void						SendEventInfoFirstClose();
	DWORD						GetEventLastActionTick();
	BOOL						DoNotExistEventDungeonEntered(DWORD dwCurrentTick, EVENT_DUNGEON_PROPERTY* pProperty);
	void						ResetEventInfo(BOOL bTimeUp);	
	BOOL						CheckEventTimes();
	void						SetDefenseCount(BYTE byDefenseCount);	

	bool						IsStadium()const{ return (DUNGEON_TYPE_MATCH_STATIDUM == m_lDungeonType);}//길드전 맵이냐?: hwoarang 050202
	bool						IsVilliage()const{ return (DUNGEON_TYPE_VILLAGE == m_lDungeonType);}//마을 맵이냐?: hwoarang 050202

	DUNGEON_DATA_EX();
	virtual ~DUNGEON_DATA_EX();
};


#endif 
