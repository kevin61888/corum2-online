#if !defined(AFX_DUNGEON_DATA_EX_H__F8906694_C290_4F80_AFC8_5C85DA83FE1C__INCLUDED_)
#define AFX_DUNGEON_DATA_EX_H__F8906694_C290_4F80_AFC8_5C85DA83FE1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 


#include "dungeon_data.h"


class CUser;

// 던젼이 클리어 되고 닫히기 까지의 딜레이.
const DWORD EVENT_DUNGEON_RESET_DELAY = 10000;


class DUNGEON_DATA_EX  : public DUNGEON_DATA
{	
	void				InitializeOperationMode();		// 숙성중인지 컨버전 중인지..

public:

	LONG				m_lDungeonType;					// 던젼 타입.
	DWORD				m_dwOriginalOwnerIndex;			// 원래 주인
	DWORD				m_dwRemainSiegeStartTick;		// 공성전 할려면 몇시간 남았는가?
	DWORD				m_dwRemainSiegeEndTick;			// 공성전 방어하기 몇시간 남았는가?
	DWORD				m_dwRemainLevelStartTick;		// 던전 레벨 (시간이 지남에 따라 레벨이 증가한다.)
	DWORD				m_dwRemainGradeCraeteTick;		// 생산품 만들어질 남은 시간.	
	BYTE				m_byEventDungeonType;

	WORD				GetLevel() const ;				// 던전 레벨
	WORD				GetUpgradeTime();				// 던전 업그레이드 시간
	int					GetIdleTime();					// 공성전 할때까지의 휴식기간.
	void				SetSiegeDestTime(DWORD dwTick);	// 공성전 될 시간 셋팅한다.
	void				DecisionDungeonType(DWORD dwID);
	int					GetDungeonType() const;
	CItem*				GetMagicFieldArrayItem();
	BOOL				LevelUpgrade();					// 던전 레벨 업그레이드하기.
	WORD				CreateProduction(BOOL bWin);	// 생산품 아아디 리턴.
	int					GetRemainTime(DWORD dwDestTime, DWORD dwCurTick, DWORD dwFlag);	// TIME_SEC, TIME_MIN, TIME_HOUR
	BOOL				IsStartWarTime();
	BOOL				IsEndWarTime();
	BOOL				IsLevelUpgradeTime();
	BOOL				IsCreateProductionTime();
	WORD				ConvertProduction();
	DWORD				GetCPTime() const;					// cp시간.
	void				SetOwner(DWORD dwOwnerIndex, DWORD dwGuildID, char* pOwnerID, char* pOwnerName);
	DWORD				GetEntrancePay();
	DWORD				GetMaxEntrancePay();
	void				SetEntrance(DWORD dwEntrance);
	int					SetAccEntrance(int nPlusMoney);	
	void				SetMagicFieldArray(CItem* pItem);
	void				SetGuardianItem(CItem* pItem);
	const CItem*		GetGuardianItem();
	void				SetOperationMode(BYTE byOperationMode);
	void				SetAccExp(DWORD dwExp);
	BOOL				IsDungeonOwner(const CUser* const pUser) const;
	void				SetDefenseCount(BYTE byDefenseCount, BOOL bDB_Update);

	BOOL				IsPathWay()const;
	BOOL				IsConquer()const;
	BOOL				IsVillage()const;
	BOOL				IsStadium()const;//길드전 맵이냐?: hwoarang 050202
	BOOL				IsEventDungeon()const;

	DUNGEON_DATA_EX();
	virtual ~DUNGEON_DATA_EX();

};
#pragma pack(pop)

#endif 

